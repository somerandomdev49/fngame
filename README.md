<h3 align="center">✨ game ✨</h3>

> 💥 *reqs:* clang (or other c++20 compiler), ninja, nlohmann_json, glm, glfw3

<hr>
<h4 align="center">- building -</h4>

buildable on WSL2. 

<pre align="center">
$ ninja
</pre>

<hr>
<h4 align="center">- running -</h4>

<pre align="center">
$ build/main
</pre>

<h4 align="center">- editor configs -</h4>
<h5 align="center">- vscode -</h5>

JSON Schemas:

```json
// Add this to your settings.json file
"json.schemas": [
    {
        "fileMatch": [
            "*.fnscene.json",
            "*.fnscene"
        ],
        "url": "./fnscene.schema.json"
    },
    {
        "fileMatch": [
            "*.fnmanifest.json",
            "*.fnmanifest"
        ],
        "url": "./fnmanifest.schema.json"
    }
],
```
