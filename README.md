<h2 align="center">✨ game ✨</h2>

> 💥 *reqs:* clang ≥11 (or other c++20 compiler), ninja, python3, nlohmann_json, glm, glfw3

<hr>
<h4 align="center">- building -</h4>

Buildable on WSL2 and MacOS ≥10.13. 

<pre align="center">
$ python tools/build.py
</pre>

<h4 align="center">- building on MacOS &lt;10.15 -</h4>

<pre align="center">
$ python=python3 custom_libcxx=1 python3 tools/build.py
</pre>

<h4 align="center">- tools/build.py options -</h4>

- Change the python executable by setting `$python`. (Default: `python`)
- Change the ninjax script by setting `$ninjax`. (Default: `tools/ninjax.py`)
- For MacOS <10.15 set `$custom_libcxx` and optionally `$custom_libcxx_path`
  to use a custom `libc++`. Default: `custom_libcxx_path` = `"/usr/local/opt/llvm/lib"`
- Set ninjax arguments by passing them into `$ninjax_args`.

<h4 align="center">- tools/ninjax.py options -</h4>

- Don't delete the `build.ninja` file by using the `nodel` flag
- Disable running ninja by using the `norun` flag
- Set a variable like so: `NAME=VALUE`
- Set the input .ninjax file with the `input` variable. (Default: `build.ninjax`)
- Set the output .ninja file with the `output` variable. (Default: `build.ninja`)
- Set the ninja executable with the `ninja` variable. (Default: `ninja`)
- Set ninja arguments with the `ninja_args` variable.

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
        "url": "./schemas/fnscene.schema.json"
    },
    {
        "fileMatch": [
            "*.fnmanifest.json",
            "*.fnmanifest"
        ],
        "url": "./schemas/fnmanifest.schema.json"
    }
],
```
