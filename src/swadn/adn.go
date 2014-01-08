// ****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2013 MASA Group
//
// ****************************************************************************
package swadn

import (
	"encoding/xml"
	"errors"
	"fmt"
	"io/ioutil"
	"path/filepath"
)

type FileHolder struct {
	File string `xml:"file,attr"`
}

type PhysicalFile struct {
	XMLName    xml.Name   `xml:"physical"`
	Components FileHolder `xml:"components"`
	Resources  FileHolder `xml:"resources"`
	Units      FileHolder `xml:"units"`
	BaseDir    string
}

func readXml(dir string, file string, result interface{}) error {
	xmlFile := filepath.Join(dir, file)
	data, err := ioutil.ReadFile(xmlFile)
	if err != nil {
		return fmt.Errorf(
			"failed to read file: %s %s", xmlFile, err)
	}
	err = xml.Unmarshal(data, result)
	return err
}

func ReadPhysical(xmlDir string) (*PhysicalFile, error) {
	result := PhysicalFile{}
	err := readXml(xmlDir, "physical.xml", &result)
	if err != nil {
		return nil, err
	}
	result.BaseDir = xmlDir
	return &result, nil
}

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

func ReadResources(physical PhysicalFile) (*Resources, error) {
	resources := Resources{}
	err := readXml(physical.BaseDir, physical.Resources.File, &resources)
	return &resources, err
}

func (r *Resources) GetResource(id uint32) *Resource {
	for _, resource := range r.Content {
		if resource.Id == id {
			return &resource
		}
	}
	return nil
}

func (r *Resources) GetIdFromName(name string) (uint32, error) {
	for _, resource := range r.Content {
		if resource.Name == name {
			return resource.Id, nil
		}
	}
	return 0, errors.New("Invalid resource name " + name + ".")
}

type UnitResource struct {
	Name        string `xml:"name,attr"`
	Consumption uint32 `xml:"normalized-consumption,attr"`
}

type Equipment struct {
	Count uint32 `xml:"count,attr"`
	Type  string `xml:"type,attr"`
}

type Unit struct {
	Equipments []Equipment `xml:"equipments>equipment"`
	Id         uint32      `xml:"id,attr"`
	Name       string      `xml:"name,attr"`
}

func (units *Units) GetName(id uint32) (string, error) {
	for _, unit := range units.Units {
		if unit.Id == id {
			return unit.Name, nil
		}
	}
	return "", errors.New("Unknown unit " + fmt.Sprintf("%d", id))
}

type Units struct {
	XMLName xml.Name `xml:"units"`
	Units   []Unit   `xml:"unit"`
}

func ReadUnits(physical PhysicalFile) (*Units, error) {
	units := Units{}
	err := readXml(physical.BaseDir, physical.Units.File, &units)
	return &units, err
}

type Component struct {
	Id           uint32         `xml:"id,attr"`
	Name         string         `xml:"name,attr"`
	Consumptions []UnitResource `xml:"composition>category>resource"`
}

type Components struct {
	XMLName    xml.Name    `xml:"equipments"`
	Components []Component `xml:"equipment"`
}

func (components *Components) Find(id string) (Component, error) {
	var result Component
	for _, component := range components.Components {
		if component.Name == id {
			return component, nil
		}
	}
	return result, errors.New("Unknown component " + id)
}

func ReadEquipments(physical PhysicalFile) (*Components, error) {
	components := Components{}
	err := readXml(physical.BaseDir, physical.Components.File, &components)
	return &components, err
}

type ResourceConsumption struct {
	Consumption map[uint32]uint32
}

type UnitConsumptions struct {
	Consumptions map[uint32]*ResourceConsumption
}

func (resources *Resources) ReadNormalizedConsumptions(physical PhysicalFile) (*UnitConsumptions, *Units, error) {
	result := UnitConsumptions{}
	result.Consumptions = map[uint32]*ResourceConsumption{}
	units, err := ReadUnits(physical)
	if err != nil {
		return nil, nil, err
	}
	equipments, err := ReadEquipments(physical)
	if err != nil {
		return nil, nil, err
	}
	for _, unit := range units.Units {
		for _, next := range unit.Equipments {
			count := next.Count
			equipment, err := equipments.Find(next.Type)
			if err != nil {
				return nil, nil, err
			}
			for _, consumption := range equipment.Consumptions {
				id, err := resources.GetIdFromName(consumption.Name)
				if err != nil {
					return nil, nil, err
				}
				if result.Consumptions[unit.Id] == nil {
					result.Consumptions[unit.Id] = &ResourceConsumption{}
				}
				if result.Consumptions[unit.Id].Consumption == nil {
					result.Consumptions[unit.Id].Consumption = make(map[uint32]uint32)
				}
				result.Consumptions[unit.Id].Consumption[id] += count * consumption.Consumption
			}
		}
	}
	return &result, units, nil
}
