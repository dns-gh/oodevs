package main

import (
	"logparser"
	"ootest"
)

func main() {
	path := "testdata/logdata/test.log"
	lines, err := logparser.FileToLines(path)
	ootest.Check(err)
	ootest.CheckBool(lines[0] == string("message test 1 [[1234]]"))
	ootest.CheckStrings(lines[1], string("message test 2 [[12.34]]"))
	value, err := logparser.GetStringValue("[[", "]]", "Test Line [[1234]]")
	ootest.Check(err)
	ootest.CheckStrings(value, string("1234"))

	m, err := logparser.GetMapValues(path)
	expectedString := []string{"1234", "12.34"}
	expectedInt := []float64{1234, 12.34}
	var index = 0
	for key, value := range *m {
		ootest.CheckStrings(key, expectedString[index])
		ootest.CheckFloat64(value, expectedInt[index])
		index++
	}
	ootest.ExitSuccess()
}
