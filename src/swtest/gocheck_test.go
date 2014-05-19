// ****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2014 MASA Group
//
// ****************************************************************************
package swtest

import (
	"io/ioutil"
	. "launchpad.net/gocheck"
	"os"
	"testing"
)

var (
	Cfg *Config
)

func init() {
	Cfg = ParseFlags()
}

func Test(t *testing.T) { TestingT(t) }

type TestSuite struct{}

var _ = Suite(&TestSuite{})

func (s *TestSuite) TestProcessReport(c *C) {
	sample, err := ioutil.ReadFile("testdata/report.txt")
	c.Assert(err, IsNil)
	expected, err := ioutil.ReadFile("testdata/expected.txt")
	c.Assert(err, IsNil)

	report := ProcessReport(string(sample))
	output := report.Config + "\n"
	for _, section := range report.Sections {
		output += section + "\n"
	}
	// Change this to update the test reference output
	if false {
		fp, err := os.Create("testdata/expected.txt")
		c.Assert(err, IsNil)
		defer fp.Close()
		_, err = fp.Write([]byte(output))
		c.Assert(err, IsNil)
	}
	AssertEqualOrDiff(c, output, string(expected))
}
