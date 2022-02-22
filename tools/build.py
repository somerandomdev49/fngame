from email.policy import default
import os, sys, platform


def get(name: str, default: str) -> str:
    return (
        os.environ[name] if name in os.environ else default
    )


def mac_version() -> float:
    return float(
        ".".join(platform.mac_ver()[0].split(".")[:2])
    )


python = get("python", "python")
ninjax = get("ninjax", "tools/ninjax.py")
ninjax_args = get("ninjax_args", "")

ninjax_env = {
    "ninja": "ninja",
    "input": "tools/build.ninjax",
    "output": "build.ninja",
}


def ninjax_args_c(env: dict[str, str]) -> str:
    return " ".join(map("=".join, env.items()))


if (
    sys.platform == "Darwin"
    and mac_version() < 10.15
    and "custom_libcxx" in os.environ
):
    custom_libcxx_path = get(
        "custom_libcxx_path",
        "/usr/local/opt/llvm/lib",
    )
    ninjax_env["OWNLIBCXX"] = custom_libcxx_path
    ninjax_env["CLANG_MACOS_VERSION_NOERROR"] = "1"

os.system(
    f"{python} {ninjax} {ninjax_args_c(ninjax_env)} {ninjax_args}"
)
