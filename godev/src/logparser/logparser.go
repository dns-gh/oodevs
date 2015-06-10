package logparser

import (
	"bufio"
	"errors"
	"ootest"
	"os"
	"strconv"
	"strings"
)

func FileToLines(path string) ([]string, error) {
	file, err := os.Open(path)
	ootest.Check(err)
	defer file.Close()

	var lines []string
	scanner := bufio.NewScanner(file)
	for scanner.Scan() {
		lines = append(lines, scanner.Text())
	}
	return lines, scanner.Err()
}

// GetStringValue returns a string from the input line located between the beacons.
func GetStringValue(leftBeacon, rightBeacon, line string) (string, error) {
	left := strings.Index(line, leftBeacon)
	if left == -1 {
		return "", errors.New("left beacon not found")
	}
	index := left + len(leftBeacon)
	right := strings.Index(line[index:], rightBeacon)
	if right == -1 {
		return "", errors.New("right beacon not found")
	}
	return line[index : index+right], nil
}

func GetMapValues(path string) (*map[string]float64, error) {
	lines, err := FileToLines(path)
	ootest.Check(err)
	m := make(map[string]float64)
	for _, v := range lines {
		val, err := GetStringValue("[[", "]]", v)
		ootest.Check(err)
		intVal, err := strconv.ParseFloat(val, 64)
		ootest.Check(err)
		m[val] += intVal
	}
	return &m, nil
}
