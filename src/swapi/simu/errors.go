// ****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2014 MASA Group
//
// ****************************************************************************
package simu

import (
	"bufio"
	"bytes"
	"fmt"
	"io"
	"io/ioutil"
	"os"
	"path/filepath"
	"regexp"
)

// Returns a list of dump files found in a dump directory.
func ListDmpFiles(path string) ([]string, error) {
	dmps := []string{}
	entries, err := ioutil.ReadDir(path)
	if err != nil {
		return dmps, err
	}
	for _, d := range entries {
		if filepath.Ext(d.Name()) == ".dmp" {
			dmps = append(dmps, filepath.Join(path, d.Name()))
		}
	}
	return dmps, nil
}

// Reads fp and possibly returns a stack trace, or an empty string.
func FindStacktrace(fp io.Reader) (string, error) {
	// [2014-01-14 11:07:28] <Simulation> <functERR> Crash -
	reStart := regexp.MustCompile(`<functERR>\s+Crash\s+-`)

	started := false
	trace := bytes.Buffer{}
	scanner := bufio.NewScanner(fp)
	for scanner.Scan() {
		line := scanner.Text()
		if !started {
			if !reStart.MatchString(line) {
				continue
			}
			started = true
		}
		trace.WriteString(line + "\n")
	}
	if err := scanner.Err(); err != nil {
		return "", err
	}
	return trace.String(), nil
}

// Checks a session directory for all kind of errors.
func CheckSessionErrors(sessionPath string) error {
	logFiles := []string{
		"debug/sim.log",
		"debug/replayer.log",
		"debug/dispatcher.log",
		"sim.log",
		"replayer.log",
		"dispatcher.log",
	}
	for _, logFile := range logFiles {
		path := filepath.Join(sessionPath, logFile)
		fp, err := os.Open(path)
		if err != nil {
			if !os.IsNotExist(err) {
				return err
			}
			continue
		}
		defer fp.Close()
		trace, err := FindStacktrace(fp)
		if err != nil {
			return err
		}
		if len(trace) > 0 {
			return fmt.Errorf("stacktrace found in %s:\n%s\n", path, trace)
		}
	}

	// We assume the debug directory is set to its default value, otherwise
	// we would have to pass all kind of options and make things much harder.
	dumpDir := filepath.Join(sessionPath, "debug")
	dumps, err := ListDmpFiles(dumpDir)
	if err != nil && !os.IsNotExist(err) {
		return err
	}
	if len(dumps) > 0 {
		s := "dump files found:\n"
		for _, dump := range dumps {
			s += "  " + dump + "\n"
		}
		return fmt.Errorf("%s", s)
	}
	return nil
}
