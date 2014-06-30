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
	"github.com/davecgh/go-spew/spew"
	"github.com/pmezard/go-difflib/difflib"
	. "launchpad.net/gocheck"
	"regexp"
	"runtime"
)

type Config struct {
	Application  string
	Platform     string
	RootDir      string
	RunDir       string
	ExercisesDir string
	ShowLog      bool
	TestPort     int
}

func ParseFlags() *Config {
	cfg := Config{}
	flag.StringVar(&cfg.Application, "application", "",
		"path to simulation_app executable")
	flag.StringVar(&cfg.RootDir, "root-dir", "",
		"path to simulation root directory")
	flag.StringVar(&cfg.RunDir, "run-dir", "",
		"path application run directory, default to application directory")
	flag.StringVar(&cfg.ExercisesDir, "exercises-dir", "",
		"exercises directory, default to root-dir/exercises")
	flag.IntVar(&cfg.TestPort, "test-port", 35000,
		"base port for spawned simulations")
	flag.BoolVar(&cfg.ShowLog, "show-log", false, "print simulation log files")

	cfg.Platform = "vc100_x64"
	if runtime.GOARCH == "386" {
		cfg.Platform = "vc100"
	}
	return &cfg
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

var (
	rePointer = regexp.MustCompile(`\(0x[0-9a-zA-Z]+\)`)
)

func Stringify(a interface{}) string {
	if s, ok := a.(string); ok {
		return s
	}
	cfg := spew.NewDefaultConfig()
	cfg.SortKeys = true
	s := cfg.Sdump(a)
	s = rePointer.ReplaceAllString(s, "")
	return s
}

func AssertEqualOrDiff(c *C, result, expected interface{}) {
	resultStr := Stringify(result)
	expectedStr := Stringify(expected)
	if expectedStr != resultStr {
		diff, err := makeDiff(expectedStr, resultStr)
		c.Assert(err, IsNil)
		c.Errorf("\n%s\n", diff)
	}
}
