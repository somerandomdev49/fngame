#include <cstdio>
#include <cstdlib>
#include <string>
#include <vector>
#include <algorithm>
#include <type_traits>
#include <iostream>
#include <fstream>
#include <sstream>
#include <map>
#include <typeindex>
#include <typeinfo>
#include <filesystem>

#include <GL/gl3w.h>
#include <GLFW/glfw3.h>

#include <entt/entity/registry.hpp>
#include <entt/fwd.hpp>

#include <nlohmann/json.hpp>

#include <glm/glm.hpp>

#include <log.hpp>
#include <fngame_json.hpp>

using namespace std::literals;

namespace graphics
{
	namespace
	{
		void errorGlfw(int errorCode, const char *msg)
		{
			srd::log::cerr << "GLFW error [" << errorCode << "]: " << msg << srd::log::endl;
		}
	} // namespace

	GLFWwindow *window;
	bool setup()
	{
		glfwSetErrorCallback(&errorGlfw);
		glfwInit();

		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
		window = glfwCreateWindow(800, 600, "fngame", NULL, NULL);
		if(window == NULL) return false;

		glfwMakeContextCurrent(window);
		gl3wInit2(&glfwGetProcAddress);

		return true;
	}

	void done() { glfwTerminate(); }

	bool shouldClose() { return glfwWindowShouldClose(window); }

	void frame()
	{
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	class Shader
	{
	};
	class Texture
	{
	};

	namespace ui
	{
		class Font
		{
		};
	} // namespace ui
} // namespace graphics

namespace base
{
	struct Transform
	{
		glm::vec2 pos;
		float rot;
		glm::vec2 scl;
	};

	namespace ui
	{
		enum class WidgetSize
		{
			Unknown,
			FitText,
			Constant,
		};

		struct Button
		{
			std::string text;
			WidgetSize size;
			graphics::ui::Font *font;
		};
	} // namespace ui
} // namespace base

namespace world
{
	struct TagComponent
	{
		std::string tag;
	};

	class Scene
	{
		entt::registry registry_;
		std::string name_;

	public:
		entt::registry &getRegistry() { return registry_; }
		const std::string &getName() const { return name_; }

		void init(const std::string &fname)
		{
			nlohmann::json j;
			std::ifstream ifs(fname);
			ifs >> j;

			loadFromJson(j);
		}

		void done() {}

		template<typename T>
		void addComponent(entt::entity e, T &&comp)
		{
			registry_.emplace<T>(e, comp);
		}

	private:
		void loadFromJson(const nlohmann::json &j);
	};

	class EntityHandle
	{
		Scene &scene_;
		entt::entity id_;

	public:
		EntityHandle(Scene &scene, entt::entity id) : scene_(scene), id_(id) {}

		Scene &getScene() { return scene_; }
		const Scene &getScene() const { return scene_; }

		entt::entity getId() const { return id_; }

		template<typename T>
		void addComponent(T &&comp)
		{
			scene_.addComponent(id_, std::move(comp));
		}
	};
} // namespace world

std::map<std::string, std::type_index> assetTypeMap = {
    {"base.ui.font"s, std::type_index(typeid(graphics::ui::Font))},
    {"base.texture"s, std::type_index(typeid(graphics::Texture))},
    {"base.shader"s, std::type_index(typeid(graphics::Shader))},
};

class AssetManager
{
public:
	void init(const std::string &manifestPath)
	{
		std::ifstream ifs(manifestPath);
		nlohmann::json j;
		ifs >> j;
		loadFromJson(j);
	}

	template<typename T>
	T *get(const std::string &name)
	{
		(void)name;
		return nullptr;
	}

private:
	void loadFromJson(const nlohmann::json &j)
	{
		auto manifest = j.get<fngame_json::FnManifest>();
		if(manifest.version != 0)
		{
			srd::log::cerr << "Bad FnManifest JSON version (must be 0): " << manifest.version << srd::log::endl;
			return;
		}

		const auto getPath = [&](const std::string &rel) {
			return std::filesystem::path(manifest.root) / std::filesystem::path(rel);
		};

		for(const auto &asset : manifest.assets)
		{
			// ...
		}
	}

	struct Res
	{
		void *data;
		bool loaded;

		Res() : data(nullptr), loaded(false) {}
		Res(void *v) : data(v), loaded(true) {}
	};

	std::map<std::type_index, std::map<std::string, Res>> data_;
} assetManager;

namespace nlohmann
{
	inline void from_json(const json &j, glm::vec2 &x)
	{
		x.x = j.at(0).get<float>();
		x.y = j.at(1).get<float>();
	}

	inline void to_json(json &j, const glm::vec2 &x)
	{
		j    = json::array();
		j[0] = x.x;
		j[1] = x.y;
	}

	inline void from_json(const json &j, base::ui::WidgetSize &x)
	{
		auto s = j.get<std::string>();
		/**/ if(s == "fit-text")
			x = base::ui::WidgetSize::FitText;
		else if(s == "constant")
			x = base::ui::WidgetSize::Constant;
		else
			x = base::ui::WidgetSize::Unknown;
	}

	inline void to_json(json &j, const base::ui::WidgetSize &x)
	{
		using E = base::ui::WidgetSize;
		switch(x)
		{
		case E::FitText: j = "fit-text"; break;
		case E::Constant: j = "constant"; break;
		case E::Unknown: [[fallthrough]];
		default: j = "unknown"; break;
		}
	}
} // namespace nlohmann

template<typename T>
void makeComponent(world::EntityHandle &e, const nlohmann::json &j);

template<>
void makeComponent<base::Transform>(world::EntityHandle &e, const nlohmann::json &j)
{
	e.addComponent<base::Transform>(base::Transform {
	    .pos = j.at("position").get<glm::vec2>(),
	    .rot = j.at("rotation").get<float>(),
	    .scl = j.at("scale").get<glm::vec2>(),
	});
}

template<>
void makeComponent<base::ui::Button>(world::EntityHandle &e, const nlohmann::json &j)
{
	e.addComponent<base::ui::Button>(base::ui::Button {
	    .text = j.at("text").get<std::string>(),
	    .size = j.at("size").get<base::ui::WidgetSize>(),
	    .font = assetManager.get<graphics::ui::Font>(j.at("font").get<std::string>()),
	});
}

std::map<std::string, std::function<void(world::EntityHandle &, const nlohmann::json &)>> componentMap = {
    {"base.transform", &makeComponent<base::Transform>},
    {"base.ui.button", &makeComponent<base::ui::Button>},
};

void world::Scene::loadFromJson(const nlohmann::json &j)
{
	auto info = j.get<fngame_json::FnGameScene>();
	if(info.version != 0)
	{
		srd::log::cerr << "Bad FnScene JSON version (must be 0): " << info.version << srd::log::endl;
		return;
	}

	name_ = info.scene.name;

	for(const auto &node : info.scene.nodes)
	{
		entt::entity e = registry_.create();
		registry_.emplace<TagComponent>(e, node.tag);

		if(node.components)
		{
			for(const auto &component : *node.components)
			{
				auto it = componentMap.find(component.type);
				if(it == componentMap.end())
				{
					srd::log::cerr << "Unknown component type: '" << component.type << "'" << srd::log::endl;
					continue;
				}

				auto eh = EntityHandle(*this, e);
				it->second(eh, component.data);
			}
		}
	}
}

int main()
{
	if(!graphics::setup()) return 1;

	world::Scene currentScene;

	currentScene.init("main_menu.json");

	while(!graphics::shouldClose())
	{

		graphics::frame();
	}

	currentScene.done();
	graphics::done();

	return 0;
}
