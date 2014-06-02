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
	"masa/sword/swapi"
	"math"
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

func Distance(pointA, pointB swapi.Point) float64 {
	return math.Sqrt(math.Pow(pointA.X-pointB.X, 2) + math.Pow(pointA.Y-pointB.Y, 2))
}

func isClose(f1, f2 float64) bool {
	return math.Abs(f1-f2) < 1e-6
}

type isCloseChecker struct {
	*CheckerInfo
}

var IsClose Checker = &isCloseChecker{
	&CheckerInfo{Name: "IsClose", Params: []string{"obtained", "expected"}},
}

func (checker *isCloseChecker) Check(params []interface{},
	names []string) (bool, string) {
	p1, ok := params[0].(float64)
	if !ok {
		return false, "first point is not a float64"
	}
	p2, ok := params[1].(float64)
	if !ok {
		return false, "second point is not a float64"
	}
	return isClose(p1, p2), ""
}

func isNearby(pointA, pointB swapi.Point) bool {
	return isClose(Distance(pointA, pointB), 0)
}

type isNearbyChecker struct {
	*CheckerInfo
}

var IsNearby Checker = &isNearbyChecker{
	&CheckerInfo{Name: "IsNearby", Params: []string{"obtained", "expected"}},
}

func (checker *isNearbyChecker) Check(params []interface{},
	names []string) (bool, string) {
	p1, ok := params[0].(swapi.Point)
	if !ok {
		return false, "first point is not a swapi.Point"
	}
	p2, ok := params[1].(swapi.Point)
	if !ok {
		return false, "second point is not a swapi.Point"
	}
	return isNearby(p1, p2), ""
}
