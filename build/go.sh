# Source this script from a Go project root to configure a development
# environment using local tools and libraries
export GOROOT=$(cygpath -m ${PWD}/bin/golang_amd64); echo GOROOT=$GOROOT
export GOPATH="$(cygpath -m ${PWD}/out/vc100_x64);$(cygpath -m ${PWD}/lib/vc100_x64);$(cygpath -m ${PWD})"; echo GOPATH=$GOPATH
export GOARCH=amd64; echo GOARCH=$GOARCH
export PATH=$(pwd)/bin/golang_amd64/bin:${PATH}
export GOSWORD_ROOT=$(cygpath -m ${PWD}); echo GOSWORD_ROOT=${GOSWORD_ROOT}
