// ****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2013 MASA Group
//
// ****************************************************************************
package main

import (
	"encoding/xml"
	"fmt"
	"io/ioutil"
)

type Resource struct {
	Id          uint32  `xml:"id,attr"`
	Name        string  `xml:"name,attr"`
	Category    string  `xml:"category,attr"`
	PackageSize uint32  `xml:"package-size,attr"`
	Mass        float64 `xml:"package-mass,attr"`
	Volume      float64 `xml:"package-volume,attr"`
}

type Resources struct {
	XMLName xml.Name   `xml:"resources"`
	Content []Resource `xml:"resource"`
}

func ReadResources(xmlFile string) (*Resources, error) {
	// resources
	// resource
	// category   id   name   package-mass   package-size   package-volume
	data, err := ioutil.ReadFile(xmlFile)
	if err != nil {
		return nil, fmt.Errorf(
			"failed to read file: %s", xmlFile)
	}
	resources := Resources{}
	err = xml.Unmarshal(data, &resources)
	if err != nil {
		return nil, fmt.Errorf("Error unmarshalling xml")
	}
	return &resources, err
}

func (r *Resources) GetResource(id uint32) (*Resource, error) {
	for _, resource := range r.Content {
		if resource.Id == id {
			return &resource, nil
		}
	}
	return nil, nil
}
