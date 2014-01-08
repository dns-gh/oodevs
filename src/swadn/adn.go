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
	"log"
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

func ReadPhysical(xmlDir string) (*PhysicalFile, error) {
	xmlFile := xmlDir + "/physical.xml"
	data, err := ioutil.ReadFile(xmlFile)
	if err != nil {
		return nil, fmt.Errorf(
			"failed to read file: %s", xmlFile)
	}
	result := PhysicalFile{}
	err = xml.Unmarshal(data, &result)
	if err != nil {
		return nil, fmt.Errorf("Error unmarshalling xml")
	}
	result.BaseDir = xmlDir
	return &result, err
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
	xmlFile := physical.BaseDir + "/" + physical.Resources.File
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

func (this *Units) GetName(id uint32) (string, error) {
	if this == nil {
		return "", errors.New("Nil Units")
	}
	for _, unit := range this.Units {
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
	xmlFile := physical.BaseDir + "/" + physical.Units.File
	data, err := ioutil.ReadFile(xmlFile)
	if err != nil {
		return nil, fmt.Errorf(
			"failed to read file: %s", xmlFile)
	}
	units := Units{}
	err = xml.Unmarshal(data, &units)
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

func (this *Components) Find(id string) (Component, error) {
	var result Component
	if this == nil {
		return result, errors.New("Nil swadn.Components")
	}
	for _, component := range this.Components {
		if component.Name == id {
			return component, nil
		}
	}
	return result, errors.New("Unknown component " + id)
}

func ReadEquipments(physical PhysicalFile) (*Components, error) {
	xmlFile := physical.BaseDir + "/" + physical.Components.File
	data, err := ioutil.ReadFile(xmlFile)
	if err != nil {
		return nil, fmt.Errorf(
			"failed to read file: %s", xmlFile)
	}
	components := Components{}
	err = xml.Unmarshal(data, &components)
	return &components, err
}

type ResourceConsumption struct {
	Consumption map[uint32]uint32
}

type UnitConsumptions struct {
	Consumptions map[uint32]*ResourceConsumption
}

func (this *Resources) ReadNormalisedConsumptions(physical PhysicalFile) (*UnitConsumptions, *Units, error) {
	result := UnitConsumptions{}
	result.Consumptions = make(map[uint32]*ResourceConsumption)
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
				id, err := this.GetIdFromName(consumption.Name)
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

func (this *UnitConsumptions) NormalisedConsumption(unitType uint32, resourceType uint32) (uint32, error) {
	if this == nil {
		return 0, errors.New("Nil UnitConsumption")
	}
	if this.Consumptions[unitType] == nil {
		log.Printf("Unknown unit " + fmt.Sprintf("%d", unitType))
		return 0, nil
	}
	if this.Consumptions[unitType].Consumption == nil {
		log.Printf("No consumptions at all for given unit")
		return 0, nil
	}
	return this.Consumptions[unitType].Consumption[resourceType], nil
}
