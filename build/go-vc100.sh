# Source this script from a Go project root to configure a development
# environment using local tools and libraries
export GOROOT=$(cygpath -m ${PWD}/bin/golang_386); echo GOROOT=$GOROOT
export GOPATH="$(cygpath -m ${PWD}/lib/vc100);$(cygpath -m ${PWD})"; echo GOPATH=$GOPATH
export PATH=$(pwd)/bin/golang_386/bin:${PATH}
