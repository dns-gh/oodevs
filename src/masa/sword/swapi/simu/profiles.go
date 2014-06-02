// ****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2013 MASA Group
//
// ****************************************************************************
package simu

import (
	"encoding/xml"
	"fmt"
	"io/ioutil"
)

type Profile struct {
	Name        string `xml:"name,attr"`
	Password    string `xml:"password,attr"`
	Supervision bool   `xml:"supervision,attr"`
}

type Profiles struct {
	Profiles []Profile `xml:"profile"`
}

func (profiles *Profiles) FindSupervisor() *Profile {
	for _, p := range profiles.Profiles {
		if p.Supervision {
			return &p
		}
	}
	return nil
}

func ReadProfiles(data []byte) (*Profiles, error) {
	p := Profiles{}
	err := xml.Unmarshal(data, &p)
	if err != nil {
		return nil, fmt.Errorf("cannot parse profiles: %v", err)
	}
	return &p, nil
}

func ReadProfilesFile(profilePath string) (*Profiles, error) {
	data, err := ioutil.ReadFile(profilePath)
	if err != nil {
		return nil, fmt.Errorf(
			"failed to read profiles file: %v", profilePath)
	}
	profiles, err := ReadProfiles(data)
	if err != nil {
		return nil, fmt.Errorf(
			"failed to parse profiles file: %v", err)
	}
	return profiles, nil
}
