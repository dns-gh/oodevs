// ****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2014 MASA Group
//
// ****************************************************************************
package swtest

import (
	"regexp"
	"strings"
)

var (
	// Strip matching lines from output report
	reNoise = regexp.MustCompile(`^(` +
		`OK:\s+.*|` +
		`OOPS:.*|` +
		`--- (?:PASS|FAIL).*|` +
		`ok\s+.*|` +
		`\?\s+.*|` +
		`PASS|` +
		`FAIL|` +
		`FAIL\s+[a-z/-]+\s+[\d\.]+s|` +
		`)\s*$`)
)

type TestReport struct {
	Config   string
	Sections []string
}

// Splits and filters the output of a single "go test" run, removes emopty
// sections and separates the "configuration" part if any from the test output
// Configuration lines contains "config:".
// See the test for a sample of the input data.
func ProcessReport(report string) *TestReport {
	parts := [][]string{}
	config := []string{}
	// Split the sections
	for _, line := range strings.Split(report, "\n") {
		if strings.HasPrefix(line, "=== RUN Test-") {
			parts = append(parts, nil)
		} else {
			if reNoise.MatchString(line) {
				continue
			}
			if len(parts) == 0 {
				continue
			}
			if strings.Contains(line, "config:") {
				config = append(config, line)
			} else {
				parts[len(parts)-1] = append(parts[len(parts)-1], line)
			}
		}
	}

	// Rebuild the sections and drop the empty ones
	sections := []string{}
	for _, part := range parts {
		section := strings.Join(part, "\n")
		if len(strings.TrimSpace(section)) == 0 {
			continue
		}
		sections = append(sections, section)
	}
	return &TestReport{
		Config:   strings.Join(config, "\n"),
		Sections: sections,
	}
}
