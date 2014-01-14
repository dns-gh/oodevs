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
	"fmt"
	"io/ioutil"
	"os"
	"path/filepath"
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

// Checks a session directory for all kind of errors.
func CheckSessionErrors(sessionPath string) error {
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
