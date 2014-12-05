// ****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2014 MASA Group
//
// ****************************************************************************
package main

import (
	"bufio"
	"flag"
	"fmt"
	"io/ioutil"
	"os"
	"path/filepath"
	"regexp"
	"sort"
	"strings"
)

var (
	reDec = regexp.MustCompile(`(?s)(?:RegisterFunction|RegisterMethod)\s*\(\s*"([^"]+)`)
)

func listFileDec(path string) ([]string, error) {
	data, err := ioutil.ReadFile(path)
	if err != nil {
		return nil, err
	}
	dec := []string{}
	matches := reDec.FindAllSubmatch(data, -1)
	if matches != nil {
		for _, m := range matches {
			dec = append(dec, string(m[1]))
		}
	}
	return dec, err
}

func listAllDecs(srcDir string) (map[string]bool, error) {
	allDecs := map[string]bool{}
	err := filepath.Walk(srcDir, func(path string, fi os.FileInfo, err error) error {
		if err != nil || fi.IsDir() {
			return err
		}
		ext := strings.ToLower(filepath.Ext(path))
		if ext != ".cpp" && ext != ".h" && ext != ".hpp" && ext != ".inl" {
			return err
		}
		decs, err := listFileDec(path)
		if err != nil {
			return err
		}
		for _, dec := range decs {
			allDecs[dec] = true
		}
		return nil
	})
	return allDecs, err
}

func parseDecList(path string) (map[string]bool, error) {
	fp, err := os.Open(path)
	if err != nil {
		return nil, err
	}
	defer fp.Close()

	decs := map[string]bool{}
	scanner := bufio.NewScanner(fp)
	for scanner.Scan() {
		line := strings.TrimSpace(scanner.Text())
		if line == "" || line[0] == '#' {
			continue
		}
		decs[line] = true
	}
	return decs, nil
}

func checkDec() error {
	flag.Usage = func() {
		fmt.Fprintf(os.Stderr, strings.TrimSpace(`
checkdec DECLIST SRCPATH

Checks that every DEC function registered in C++ files within SRCPATH are
listed in DECLIST, but for those prefixed by DEC_.
`))
		flag.PrintDefaults()
	}
	flag.Parse()
	if flag.NArg() < 1 {
		return fmt.Errorf("DEC file file expected")
	}
	if flag.NArg() < 2 {
		return fmt.Errorf("source files directory expected")
	}
	decList := flag.Arg(0)
	srcDir := flag.Arg(1)

	known, err := parseDecList(decList)
	if err != nil {
		return err
	}
	decs, err := listAllDecs(srcDir)
	if err != nil {
		return err
	}
	if len(decs) == 0 {
		return fmt.Errorf("no DEC function in source code")
	}
	// Cannot blame models for using this one
	delete(decs, "LoadResourcesFile")
	// Filter out well known DEC functions
	for k := range decs {
		if strings.HasPrefix(k, "DEC_") || strings.HasPrefix(k, "_DEC_") {
			delete(decs, k)
		}
	}
	// models list is a superset of actual DEC set
	missing := []string{}
	for actual := range decs {
		if !known[actual] {
			missing = append(missing, actual)
		}
	}
	if len(missing) == 0 {
		return nil
	}
	sort.Strings(missing)
	fmt.Fprintf(os.Stderr, strings.TrimSpace(`
error: upstream mode project does not know about several DEC functions
       defined by the simulation. Please append them to model
       decfunctions.txt reference file
`)+"\n\n")
	for _, name := range missing {
		fmt.Fprintf(os.Stderr, "  %s\n", name)
	}
	return fmt.Errorf("")
}

func main() {
	err := checkDec()
	if err != nil {
		if err.Error() != "" {
			fmt.Fprintf(os.Stderr, "error: %s\n", err)
		}
		os.Exit(1)
	}
}
