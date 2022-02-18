#!/bin/sh

python=${python:-"py"}
ninjax=${ninjax:-"tools/ninjax.py"}

_argd="ninja=ninja input=tools/build.ninjax output=build.ninja nodel"
if [[ $(uname) -eq "Darwin" ]]; then
    # steal from https://gist.github.com/scriptingosx/670991d7ec2661605f4e3a40da0e37aa
    os_ver=${1:-$(sw_vers -productVersion)}
    IFS='.' read -r -a ver <<< "$os_ver"
    # end steal
    
    if [[ "${ver[1]}" -le 15 ]] && [[ -n "${custom_libcxx:+1}" ]]; then
        custom_libcxx_path=${custom_libcxx_path:-"/usr/local/opt/llvm/lib"}
        $python $ninjax $_argd OWNLIBCXX=$custom_libcxx_path CLANG_MACOS_VERSION_NOERROR=1 $ninjax_args
    else
        $python $ninjax $_argd $ninjax_args
    fi
else
    $python $ninjax $ninjax_default_args $ninjax_args
fi
