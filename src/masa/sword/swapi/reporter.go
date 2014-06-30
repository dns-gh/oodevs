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
	"masa/sword/swapi/phy"
	"masa/sword/sword"
	"strings"
	"sync"
	"time"
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
// one message. The report identifier to pattern is also returned to help
// generating traces from matched reports.
func ReportMessages(db *phy.PhysicalFile, patterns ...string) (
	func(*sword.Report) bool, map[uint32]string, error) {

	reports, err := phy.ReadReports(*db)
	if err != nil {
		return nil, nil, err
	}
	ids := map[uint32]string{}
	for _, pattern := range patterns {
		report := reports.MatchUniqueByMessage(pattern)
		if report == nil {
			return nil, nil, fmt.Errorf(
				"reports matching '%s' are either missing or non-unique", pattern)
		}
		ids[report.Id] = pattern
	}
	filter := func(r *sword.Report) bool {
		_, ok := ids[r.Type.GetId()]
		return ok
	}
	return filter, ids, nil
}

// Take a list of matched reports and return a string where each line is the
// original pattern used to match the report, provided to ReportMessages.
func PrintReports(reports []*sword.Report, patterns map[uint32]string) string {
	lines := []string{}
	for _, report := range reports {
		if report == nil {
			lines = append(lines, "\n")
		} else if pattern, ok := patterns[*report.Type.Id]; ok {
			lines = append(lines, pattern+"\n")
		} else {
			lines = append(lines, "???\n")
		}
	}
	return strings.Join(lines, "")
}

type ReporterCond func([]*sword.Report) bool

// A reporter listens to all incoming reports and records those matched by
// supplied filter. Records are obtained when stopping the recorder.
// Timeout controls Wait() timeout and should be changed before recording
// reports.
type Reporter struct {
	Timeout time.Duration

	lock      *sync.Mutex // Protects reports
	reports   []*sword.Report
	filter    func(*sword.Report) bool
	handlerId int32
	model     *Model
	conds     []ReporterCond
}

func NewReporter(filter func(*sword.Report) bool) *Reporter {
	return &Reporter{
		Timeout: 40 * time.Second,
		lock:    &sync.Mutex{},
		filter:  filter,
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
				r.lock.Lock()
				defer r.lock.Unlock()
				r.reports = append(r.reports, report)
				for i := 0; i < len(r.conds); {
					if r.conds[i](r.reports) {
						r.conds = append(r.conds[:i], r.conds[i+1:]...)
					} else {
						i++
					}
				}
			}
			return false
		})
}

// Returns the number of reports caught until now.
func (r *Reporter) Count() int {
	r.lock.Lock()
	defer r.lock.Unlock()
	return len(r.reports)
}

// Waits until the number of caught reports equals or exceeds "count". Returns
// false on timeout.
func (r *Reporter) WaitCount(count int) bool {
	done := make(chan bool, 2)
	r.lock.Lock()
	fn := func(reports []*sword.Report) bool {
		if len(reports) >= count {
			done <- true
			return true
		}
		return false
	}
	r.conds = append(r.conds, fn)
	r.lock.Unlock()

	go func() {
		time.Sleep(r.Timeout)
		done <- false
	}()

	return <-done
}

// Append a nil Report entry to the lock list. Used to explicitely separate
// sequence of reports before printing them.
func (r *Reporter) AddNilReport() {
	r.lock.Lock()
	defer r.lock.Unlock()
	r.reports = append(r.reports, nil)
}

// Stop recording reports and return the list of matched ones. The list is not
// cleared, calling Start() again will append more reports to the list.
func (r *Reporter) Stop() []*sword.Report {
	if r.handlerId != 0 {
		r.model.UnregisterHandler(r.handlerId)
		r.handlerId = 0
	}
	// Report slice may be used again later if caller Start() again
	r.lock.Lock()
	defer r.lock.Unlock()
	copies := make([]*sword.Report, len(r.reports))
	copy(copies, r.reports)
	return copies
}
