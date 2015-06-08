package main

import (
	"logparser"
	"ootest"
)

func main() {
	lines, err := logparser.FileToLines("testdata/logdata/test.log")
	ootest.Check(err)
	ootest.CheckBool(lines[0] == string("message test 1 [[ inside message; 1234 ]]"))
	ootest.CheckStrings(lines[1], string("message test 2 [[ inside message; 12.34 ]]"))
	ootest.ExitSuccess()
}
