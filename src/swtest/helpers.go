// ****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2013 MASA Group
//
// ****************************************************************************
package swtest

import (
	"flag"
	"github.com/pmezard/go-difflib/difflib"
	. "launchpad.net/gocheck"
	"runtime"
)

func InitFlag(application, rootdir, rundir, platform *string, testPort *int,
	showLog *bool) {
	flag.StringVar(application, "application", "",
		"path to simulation_app executable")
	flag.StringVar(rootdir, "root-dir", "",
		"path to simulation root directory")
	flag.StringVar(rundir, "run-dir", "",
		"path application run directory, default to application directory")
	flag.IntVar(testPort, "test-port", 35000,
		"base port for spawned simulations")
	flag.BoolVar(showLog, "show-log", false, "print simulation log files")

	*platform = "vc100_x64"
	if runtime.GOARCH == "386" {
		*platform = "vc100"
	}
}

func makeDiff(before, after string) (string, error) {
	diff := difflib.UnifiedDiff{
		A:        difflib.SplitLines(before),
		FromFile: "before",
		B:        difflib.SplitLines(after),
		ToFile:   "after",
		Context:  4,
	}
	return difflib.GetUnifiedDiffString(diff)
}

func AssertEqualOrDiff(c *C, result, expected string) {
	if expected != result {
		diff, err := makeDiff(expected, result)
		c.Assert(err, IsNil)
		c.Errorf("\n%s\n", diff)
	}
}
