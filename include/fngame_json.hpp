// Generated by https://app.quicktype.io/ :D
//              ^^^^^^^^^^^^^^^^^^^^^^^^^
//                      very cool
//
// + some additional modifications by me

#pragma once
#include <vector>
#include <string>
#include <memory>

#include <nlohmann/json.hpp>


#ifndef NLOHMANN_OPT_HELPER
#define NLOHMANN_OPT_HELPER
namespace nlohmann
{
	template<typename T>
	struct adl_serializer<std::shared_ptr<T>>
	{
		static void to_json(json &j, const std::shared_ptr<T> &opt)
		{
			if(!opt) j = nullptr;
			else j = *opt;
		}

		static std::shared_ptr<T> from_json(const json &j)
		{
			if(j.is_null()) return std::unique_ptr<T>();
			else return std::unique_ptr<T>(new T(j.get<T>()));
		}
	};
} // namespace nlohmann
#endif

namespace fngame_json
{
	using nlohmann::json;

	inline json get_untyped(const json &j, const char *property)
	{
		if(j.find(property) != j.end())
		{
			return j.at(property).get<json>();
		}
		return json();
	}

	inline json get_untyped(const json &j, std::string property) { return get_untyped(j, property.data()); }

	template<typename T>
	inline std::shared_ptr<T> get_optional(const json &j, const char *property)
	{
		if(j.find(property) != j.end())
		{
			return j.at(property).get<std::shared_ptr<T>>();
		}
		return std::shared_ptr<T>();
	}

	template<typename T>
	inline std::shared_ptr<T> get_optional(const json &j, std::string property)
	{
		return get_optional<T>(j, property.data());
	}

	struct Component
	{
		/**
		 * The component's registered type
		 */
		std::string type;

		/**
		 * Additional component data
		 */
		nlohmann::json data;
	};

	/**
	 * A node of a scene
	 */
	struct Node
	{
		/**
		 * The node's components
		 */
		std::shared_ptr<std::vector<Component>> components;
		/**
		 * The node's tag
		 */
		std::string tag;
	};

	/**
	 * Scene info
	 */
	struct Scene
	{
		/**
		 * Scene name
		 */
		std::string name;
		/**
		 * The nodes of the scene
		 */
		std::vector<Node> nodes;
	};

	/**
	 * Scene description object
	 */
	struct FnGameScene
	{
		/**
		 * Scene info
		 */
		Scene scene;
		/**
		 * Scene info schema version
		 */
		int64_t version;
	};
} // namespace fngame_json

namespace nlohmann
{
	void from_json(const json &j, fngame_json::Component &x);
	void to_json(json &j, const fngame_json::Component &x);

	void from_json(const json &j, fngame_json::Node &x);
	void to_json(json &j, const fngame_json::Node &x);

	void from_json(const json &j, fngame_json::Scene &x);
	void to_json(json &j, const fngame_json::Scene &x);

	void from_json(const json &j, fngame_json::FnGameScene &x);
	void to_json(json &j, const fngame_json::FnGameScene &x);

	inline void from_json(const json &j, fngame_json::Component &x)
	{
		x.data = j;
		x.type = j.at("@type").get<std::string>();
	}

	inline void to_json(json &j, const fngame_json::Component &x)
	{
		j          = json(x.data);
		j["@type"] = x.type;
	}

	inline void from_json(const json &j, fngame_json::Node &x)
	{
		x.components = fngame_json::get_optional<std::vector<fngame_json::Component>>(j, "components");
		x.tag        = j.at("tag").get<std::string>();
	}

	inline void to_json(json &j, const fngame_json::Node &x)
	{
		j               = json::object();
		j["components"] = x.components;
		j["tag"]        = x.tag;
	}

	inline void from_json(const json &j, fngame_json::Scene &x)
	{
		x.name  = j.at("name").get<std::string>();
		x.nodes = j.at("nodes").get<std::vector<fngame_json::Node>>();
	}

	inline void to_json(json &j, const fngame_json::Scene &x)
	{
		j          = json::object();
		j["name"]  = x.name;
		j["nodes"] = x.nodes;
	}

	inline void from_json(const json &j, fngame_json::FnGameScene &x)
	{
		x.scene   = j.at("scene").get<fngame_json::Scene>();
		x.version = j.at("version").get<int64_t>();
	}

	inline void to_json(json &j, const fngame_json::FnGameScene &x)
	{
		j            = json::object();
		j["scene"]   = x.scene;
		j["version"] = x.version;
	}
} // namespace nlohmann