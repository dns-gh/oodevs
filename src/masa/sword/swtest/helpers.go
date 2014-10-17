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
	"launchpad.net/gocheck"
	"os"
	"regexp"
	"runtime"
	"strings"
	"time"
)

type Config struct {
	Application  string
	BaseDir      string // base directory from which all other may be deduced
	Platform     string
	RootDir      string // simulation --root-dir, usually "data/"
	RunDir       string
	ExercisesDir string
	ShowLog      bool
	TestPort     int
	Timeout      time.Duration
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

	cfg.Timeout = 5 * time.Second
	timeoutStr := os.Getenv("MASA_TIMEOUT")
	if timeoutStr != "" {
		t, err := time.ParseDuration(timeoutStr)
		if err == nil {
			cfg.Timeout = t
		}
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
	reCap     = regexp.MustCompile(`\((len=\d+) cap=\d+\)`)
)

func Stringify(a interface{}) string {
	if s, ok := a.(string); ok {
		return s
	}
	cfg := spew.NewDefaultConfig()
	cfg.SortKeys = true
	s := cfg.Sdump(a)
	s = rePointer.ReplaceAllString(s, "")
	s = reCap.ReplaceAllString(s, `$1`)
	return s
}

func DeepEquals(c *gocheck.C, result, expected interface{}) {
	resultStr := Stringify(result)
	expectedStr := Stringify(expected)
	if expectedStr != resultStr {
		diff, err := makeDiff(expectedStr, resultStr)
		c.Assert(err, gocheck.IsNil)
		c.Errorf("\n%s\n", diff)
	}
}

type TestLogger struct {
	c *gocheck.C
}

func (t *TestLogger) Printf(format string, a ...interface{}) {
	fmt := time.Now().Format(time.Stamp) + ": " + strings.TrimSpace(format)
	t.c.Logf(fmt, a...)
}

func MakeGocheckLogger(c *gocheck.C) *TestLogger {
	return &TestLogger{c: c}
}
