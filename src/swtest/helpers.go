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
	"github.com/pmezard/go-difflib/difflib"
	. "launchpad.net/gocheck"
)

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
