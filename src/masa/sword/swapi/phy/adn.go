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
	"encoding/xml"
	"errors"
	"fmt"
	"io/ioutil"
	"path/filepath"
	"regexp"
)

type FileHolder struct {
	File string `xml:"file,attr"`
}

type PhysicalFile struct {
	XMLName    xml.Name   `xml:"physical"`
	Automats   FileHolder `xml:"automats"`
	Components FileHolder `xml:"components"`
	Reports    FileHolder `xml:"reports"`
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
	Capacity    uint32 `xml:"capacity,attr"`
}

type Equipment struct {
	Count uint32 `xml:"count,attr"`
	Type  string `xml:"type,attr"`
}

type Automat struct {
	Id   uint32 `xml:"id,attr"`
	Name string `xml:"name,attr"`
}

func (automats *Automats) GetName(id uint32) (string, error) {
	for _, automat := range automats.Automats {
		if automat.Id == id {
			return automat.Name, nil
		}
	}
	return "", errors.New("Unknown automat " + fmt.Sprintf("%d", id))
}

type Automats struct {
	XMLName  xml.Name  `xml:"automats"`
	Automats []Automat `xml:"automat"`
}

func ReadAutomats(physical PhysicalFile) (*Automats, error) {
	automats := Automats{}
	err := readXml(physical.BaseDir, physical.Automats.File, &automats)
	return &automats, err
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
	Capacities   map[uint32]*ResourceConsumption
}

func (resources *Resources) ReadNormalizedConsumptions(physical PhysicalFile) (*UnitConsumptions, *Units, error) {
	result := UnitConsumptions{}
	result.Consumptions = map[uint32]*ResourceConsumption{}
	result.Capacities = map[uint32]*ResourceConsumption{}
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
				currentConsumptions := result.Consumptions[unit.Id]
				if currentConsumptions == nil {
					currentConsumptions = &ResourceConsumption{Consumption: map[uint32]uint32{}}
					result.Consumptions[unit.Id] = currentConsumptions
				}
				currentConsumptions.Consumption[id] += count * consumption.Consumption
				currentCapacities := result.Capacities[unit.Id]
				if currentCapacities == nil {
					currentCapacities = &ResourceConsumption{Consumption: map[uint32]uint32{}}
					result.Capacities[unit.Id] = currentCapacities
				}
				currentCapacities.Consumption[id] += count * consumption.Capacity
			}
		}
	}
	return &result, units, nil
}

type Report struct {
	Id      uint32 `xml:"id,attr"`
	Key     string `xml:"key,attr"`
	Message string `xml:"message,attr"`
}

type Reports []Report

// Returns all reports which message match supplied regular expression. Returns
// an empty slice if none is found or the pattern is invalid.
func (r *Reports) MatchByMessage(pattern string) Reports {
	reports := Reports{}
	re, err := regexp.Compile(pattern)
	if err != nil {
		return Reports(reports)
	}
	for _, report := range *r {
		if re.MatchString(report.Message) {
			reports = append(reports, report)
		}
	}
	return reports
}

// Returns the only message matching supplied pattern, nil otherwise.
func (r *Reports) MatchUniqueByMessage(regexp string) *Report {
	matched := r.MatchByMessage(regexp)
	if len(matched) != 1 {
		return nil
	}
	return &matched[0]
}

type xmlReports struct {
	XMLName xml.Name `xml:"reports"`
	Reports []Report `xml:"report"`
}

func ReadReports(physical PhysicalFile) (Reports, error) {
	xml := xmlReports{}
	err := readXml(physical.BaseDir, physical.Reports.File, &xml)
	if err != nil {
		return Reports{}, err
	}
	return Reports(xml.Reports), nil
}
