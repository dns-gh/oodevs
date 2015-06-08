package ootest

import (
	"errors"
	"fmt"
	"log"
	"os"
	"path"
	"runtime"
	"strings"
)

func ExitSuccess() {
	fmt.Println("Success !")
	os.Exit(0)
}

func ExitFailure() {
	fmt.Println("Failure !")
	os.Exit(1)
}

func PrintError(err error, depth int) {
	if err != nil {
		_, fn, line, _ := runtime.Caller(depth)
		filename := path.Base(fn)
		log.Printf("[error] in %s:%d : %v", filename, line, err)
	}
}

func Check(err error) {
	if err != nil {
		PrintError(err, 2)
		ExitFailure()
	}
}

func CheckBool(value bool) {
	if !value {
		PrintError(errors.New("not the whole truth !"), 2)
		ExitFailure()
	}
}

func CheckStrings(lhs, rhs string) {
	if len(lhs) != len(rhs) {
		PrintError(errors.New(lhs+" != "+rhs), 2)
		ExitFailure()
	}
	if !strings.Contains(lhs, rhs) {
		PrintError(errors.New(lhs+" != "+rhs), 2)
		ExitFailure()
	}
}
