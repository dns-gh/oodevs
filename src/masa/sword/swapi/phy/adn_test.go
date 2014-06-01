// ****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2013 MASA Group
//
// ****************************************************************************
package phy

import (
	. "launchpad.net/gocheck"
	"os"
	"path/filepath"
	"swtest"
	"testing"
)

var (
	Cfg *swtest.Config
)

func init() {
	Cfg = swtest.ParseFlags()
}

func Test(t *testing.T) { TestingT(t) }

type TestSuite struct{}

var _ = Suite(&TestSuite{})

func LoadWWPhysical(c *C) *PhysicalFile {
	wd, err := os.Getwd()
	c.Assert(err, IsNil)
	path := filepath.Join(wd, "../../../data/data/models/ada/physical/worldwide")
	phydb, err := ReadPhysical(path)
	c.Assert(err, IsNil)
	return phydb
}

func (s *TestSuite) TestReports(c *C) {
	ww := LoadWWPhysical(c)
	reports, err := ReadReports(*ww)
	c.Assert(err, IsNil)
	c.Assert(len(reports), Greater, 0)

	// Invalid pattern
	matched := reports.MatchByMessage("missing[ closing brace")
	c.Assert(len(matched), Equals, 0)

	// No match
	matched = reports.MatchByMessage("probably not in any report")
	c.Assert(len(matched), Equals, 0)

	// Multiple matches
	matched = reports.MatchByMessage("object")
	c.Assert(len(matched), Greater, 1)
	unique := reports.MatchUniqueByMessage("object")
	c.Assert(unique, IsNil)

	// Single match
	matched = reports.MatchByMessage("Hard to go through")
	c.Assert(len(matched), Equals, 1)
	unique = reports.MatchUniqueByMessage("Hard to go through")
	c.Assert(unique, NotNil)
}
