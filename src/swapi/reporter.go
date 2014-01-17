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
	"fmt"
	"swapi/phy"
	"sword"
)

// Creates a report filter matching supplied entity identifiers.
func ReportSources(sources ...uint32) func(*sword.Report) bool {
	ids := map[uint32]struct{}{}
	for _, id := range sources {
		ids[id] = struct{}{}
	}
	return func(r *sword.Report) bool {
		_, ok := ids[GetTaskerId(r.GetSource())]
		return ok
	}
}

// Creates a report filter matching the identifiers of reports uniquely
// matched by supplied patterns. Fail if a pattern matches zero or more than
// one message.
func ReportMessages(db *phy.PhysicalFile, patterns ...string) (
	func(*sword.Report) bool, error) {

	reports, err := phy.ReadReports(*db)
	if err != nil {
		return nil, err
	}
	ids := map[uint32]struct{}{}
	for _, pattern := range patterns {
		report := reports.MatchUniqueByMessage(pattern)
		if report == nil {
			return nil, fmt.Errorf(
				"reports matching '%s' are either missing or non-unique", pattern)
		}
		ids[report.Id] = struct{}{}
	}
	filter := func(r *sword.Report) bool {
		_, ok := ids[r.Type.GetId()]
		return ok
	}
	return filter, nil
}

// A reporter listens to all incoming reports and records those matched by
// supplied filter. Records are obtained when stopping the recorder.
type Reporter struct {
	reports   []*sword.Report
	filter    func(*sword.Report) bool
	handlerId int32
	model     *Model
}

func NewReporter(filter func(*sword.Report) bool) *Reporter {
	return &Reporter{
		filter: filter,
	}
}

// Start recording reports matching the filter.
func (r *Reporter) Start(model *Model) {
	r.Stop()
	r.model = model
	r.handlerId = model.RegisterHandlerTimeout(0,
		func(m *ModelData, msg *SwordMessage, err error) bool {
			if err != nil {
				return false
			}
			sim := msg.SimulationToClient
			if sim == nil || sim.Message == nil {
				return false
			}
			report := sim.Message.Report
			if report == nil {
				return false
			}
			if r.filter(report) {
				r.reports = append(r.reports, report)
			}
			return false
		})
}

// Stop recording reports and return the list of matched ones. The list is not
// cleared, calling Start() again will append more reports to the list.
func (r *Reporter) Stop() []*sword.Report {
	if r.handlerId != 0 {
		r.model.UnregisterHandler(r.handlerId)
		r.handlerId = 0
	}
	// Report slice may be used again later if caller Start() again
	copies := make([]*sword.Report, len(r.reports))
	copy(copies, r.reports)
	return copies
}
