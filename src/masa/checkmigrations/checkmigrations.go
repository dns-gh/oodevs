// ****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2014 MASA Group
//
// ****************************************************************************
package main

import (
	"encoding/xml"
	"fmt"
	"io/ioutil"
	"os"
	"path/filepath"
	"regexp"
	"sort"
	"strings"
)

type Upgrade struct {
	Apply string `xml:"apply,attr"`
}

type Migrations struct {
	XMLName  xml.Name  `xml:"migrations"`
	Upgrades []Upgrade `xml:"migration>upgrade"`
}

// Lists all XSL files referenced in supplied migrations.xml.
func listXsl(migrationsFile string) ([]string, error) {
	fp, err := os.Open(migrationsFile)
	if err != nil {
		return nil, err
	}
	defer fp.Close()

	x := &Migrations{}
	err = xml.NewDecoder(fp).Decode(x)
	if err != nil {
		return nil, err
	}
	files := []string{}
	for _, u := range x.Upgrades {
		files = append(files, u.Apply)
	}
	sort.Strings(files)
	return files, nil
}

var (
	// <xsl:import href="./tools.xsl"/>
	reImport = regexp.MustCompile(`<xsl:import href="([^\"]+)`)
)

// Lists files imported by "xsl".
func listImportedXsl(xsl string) ([]string, error) {
	data, err := ioutil.ReadFile(xsl)
	if err != nil {
		return nil, err
	}
	dir := filepath.Dir(xsl)
	m := reImport.FindAllSubmatch(data, -1)
	imported := []string{}
	for _, s := range m {
		path := string(s[1])
		// Assume path relative to xsl directory, unlikely to work otherwise
		path = filepath.Join(dir, path)
		imported = append(imported, path)
	}
	return imported, nil
}

func normPath(path, base string) (string, error) {
	rel, err := filepath.Rel(base, path)
	if err != nil {
		return "", err
	}
	rel = strings.ToLower(filepath.ToSlash(rel))
	return rel, nil
}

// Checks XSL files referenced in migrations.xml exist and existing XSL files
// in "schemas" subtree are referenced in migrations.xml.
func checkXslExist(xsls []string, resourcesDir string) error {
	// List existing XSLs to cross-check with referenced ones
	existing := map[string]struct{}{}
	existingSorted := []string{}
	allImported := map[string]struct{}{}
	schemasDir := filepath.Join(resourcesDir, "schemas")
	err := filepath.Walk(schemasDir, func(path string, fi os.FileInfo, err error) error {
		if err != nil || fi.IsDir() || filepath.Ext(path) != ".xsl" {
			return err
		}
		rel, err := normPath(path, resourcesDir)
		if err != nil {
			return err
		}
		existing[rel] = struct{}{}
		existingSorted = append(existingSorted, rel)

		// Populate set of files referenced by other XSLs
		imported, err := listImportedXsl(path)
		if err != nil {
			return err
		}
		for _, imp := range imported {
			rel, err := normPath(imp, resourcesDir)
			if err != nil {
				return err
			}
			allImported[rel] = struct{}{}
		}
		return nil
	})
	if err != nil {
		return err
	}
	sort.Strings(existingSorted)

	failed := false
	referenced := map[string]struct{}{}
	for _, xsl := range xsls {
		xsl := strings.ToLower(xsl)
		if _, ok := existing[xsl]; !ok {
			failed = true
			fmt.Fprintf(os.Stderr, "error: %s is in migrations.xml but does not exist\n", xsl)
		}
		referenced[xsl] = struct{}{}
	}

	for xsl := range allImported {
		referenced[xsl] = struct{}{}
	}

	for _, xsl := range existingSorted {
		if _, ok := referenced[xsl]; !ok {
			failed = true
			fmt.Fprintf(os.Stderr, "error: %s exists but is not in migrations.xml\n", xsl)
		}
	}
	if failed {
		return fmt.Errorf("not all XSLs exist or are referenced")
	}
	return nil
}

func checkMigrations() error {
	args := os.Args[1:]
	if len(args) < 1 {
		fmt.Fprintf(os.Stderr, ""+
			`Lists XSL referenced in migrations.xml and checks they exist. Also checks
existing XSL files are referenced in migrations.xml.

`)
		return fmt.Errorf(`not enough arguments.
RESOURCESDIR expected`)
	}
	resourcesDir := args[0]
	migrationsFile := filepath.Join(resourcesDir, "migrations.xml")
	xsls, err := listXsl(migrationsFile)
	if err != nil {
		return err
	}
	return checkXslExist(xsls, resourcesDir)
}

func main() {
	err := checkMigrations()
	if err != nil {
		fmt.Fprintf(os.Stderr, "error: %s\n", err)
		os.Exit(1)
	}
}
