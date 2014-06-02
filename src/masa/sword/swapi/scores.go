// ****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2013 MASA Group
//
// ****************************************************************************
package swapi

import (
	"encoding/xml"
	"fmt"
)

type xmlScore struct {
	Name string `xml:"name,attr"`
}

type xmlScores struct {
	XMLName xml.Name   `xml:"scores"`
	Scores  []xmlScore `xml:"score"`
}

func ReadScores(data []byte) (map[string]struct{}, error) {
	x := xmlScores{}
	err := xml.Unmarshal(data, &x)
	if err != nil {
		return nil, fmt.Errorf("cannot parse scores: %v", err)
	}
	scores := map[string]struct{}{}
	for _, s := range x.Scores {
		scores[s.Name] = struct{}{}
	}
	return scores, nil
}
