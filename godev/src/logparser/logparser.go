package logparser

import (
	"bufio"
	"ootest"
	"os"
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
