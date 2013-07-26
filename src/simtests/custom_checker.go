// ****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2013 MASA Group
//
// ****************************************************************************
package simtests

import (
	. "launchpad.net/gocheck"
	"swapi"
)

type isSwordErrorChecker struct {
	*CheckerInfo
}

// The IsSwordError checker checks the input is a SwordError containing
// specified error code. Use ErrorMatches to check the error message at the
// same time.
var IsSwordError Checker = &isSwordErrorChecker{
	&CheckerInfo{Name: "IsSwordError", Params: []string{"obtained", "expected"}},
}

func (checker *isSwordErrorChecker) Check(params []interface{},
	names []string) (bool, string) {
	err, ok := params[0].(*swapi.SwordError)
	if !ok {
		return false, "obtained value is not an error"
	}
	s, ok := params[1].(string)
	if !ok {
		return false, "matched value must be a string"
	}
	return err.Name == s, ""
}
