// ****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2015 MASA Group
//
// ****************************************************************************
package services

import (
	gouuid "code.google.com/p/go-uuid/uuid"
	"code.google.com/p/goprotobuf/proto"
	"encoding/json"
	"masa/sword/swapi"
	"masa/sword/sword"
	"masa/timeline/sdk"
	"masa/timeline/util"
	"net/http"
	"net/url"
	"time"
)

var (
	ReplayRangeUuid = gouuid.New()
)

func checkTime(parameter string, replayBegin, replayEnd time.Time) (eventTime time.Time, err error) {
	eventTime, err = util.ParseTime(parameter)
	if err != nil {
		return
	}
	if eventTime.Before(replayBegin) || eventTime.After(replayEnd) {
		err = util.NewError(http.StatusBadRequest, "Event time "+parameter+" is out of replay boundaries")
	}
	return
}

func checkBoundaries(event *sdk.Event, replayBegin, replayEnd time.Time) (begin, end time.Time, err error) {
	begin, err = checkTime(event.GetBegin(), replayBegin, replayEnd)
	if err != nil {
		return
	}
	end, err = checkTime(event.GetEnd(), replayBegin, replayEnd)
	return
}

func marshal(begin, end, enabled bool) []byte {
	buffer, _ := json.Marshal(&sdk.ReplayPayload{Begin: proto.Bool(begin), End: proto.Bool(end), Enabled: proto.Bool(enabled)})
	return buffer
}

func readPayload(event *sdk.Event) (sdk.ReplayPayload, error) {
	payload := sdk.ReplayPayload{}
	err := json.Unmarshal(event.Action.Payload, &payload)
	return payload, err
}

func updatePayload(index, size int, event *sdk.Event) {
	begin := size > 1 && index != 0
	end := size > 1 && index+1 < size
	payload := sdk.ReplayPayload{}
	json.Unmarshal(event.Action.Payload, &payload)
	enabled := true
	if payload.Enabled != nil {
		enabled = payload.GetEnabled()
	}
	event.Action.Payload = marshal(begin, end, enabled)
}

func applyModification(update *sdk.Event, events *[]*sdk.Event) {
	index := len(*events)
	updateBegin, _ := util.ParseTime(update.GetBegin())
	for i, event := range *events {
		// event found and modified
		if update.GetUuid() == event.GetUuid() {
			(*events)[i] = CloneEvent(update)
			return
		}
		begin, _ := util.ParseTime(event.GetBegin())
		if begin.After(updateBegin) {
			index = i
			break
		}
	}
	// insert the event
	*events = append((*events)[:index], append([]*sdk.Event{update}, (*events)[index:]...)...)
}

func checkContinuity(start, end time.Time, events []*sdk.Event) bool {
	begin := start
	for _, event := range events {
		eventBegin, _ := util.ParseTime(event.GetBegin())
		eventEnd, _ := util.ParseTime(event.GetEnd())
		if eventBegin != begin {
			return false
		}
		begin = eventEnd
	}
	return begin == end
}

func (s *Sword) checkEventModification(event *sdk.Event, eventBegin, eventEnd time.Time) (bool, error) {
	for index, replay := range s.replays {
		if event.GetUuid() == replay.GetUuid() {
			if replay.GetBegin() != event.GetBegin() && replay.GetEnd() != event.GetEnd() {
				return false, util.NewError(http.StatusBadRequest, "Cannot modify both boundaries")
			}
			if index == 0 && eventBegin.After(s.startTime) || index == len(s.replays)-1 && eventEnd.Before(s.endTime) {
				return false, util.NewError(http.StatusBadRequest, "Cannot create gap between events and replay boundaries")
			}
			if index > 0 {
				previousReplay, _ := util.ParseTime(s.replays[index-1].GetBegin())
				if previousReplay.After(eventBegin) {
					return false, util.NewError(http.StatusBadRequest, "Cannot overlap more than one event")
				}
			}
			if index < len(s.replays)-1 {
				nextEnd, _ := util.ParseTime(s.replays[index+1].GetEnd())
				if nextEnd.Before(eventEnd) {
					return false, util.NewError(http.StatusBadRequest, "Cannot overlap more than one event")
				}
			}
			return true, nil
		}
	}
	return false, nil
}

func (s *Sword) checkEventInsertion(event *sdk.Event, eventBegin, eventEnd time.Time) bool {
	// Checks new replay event (see it like a split of an existing replay event)
	for _, replay := range s.replays {
		replayBegin, _ := util.ParseTime(replay.GetBegin())
		replayEnd, _ := util.ParseTime(replay.GetEnd())
		// Checks the split, the new event takes a part and the old event the other
		if replayBegin == eventBegin && replayEnd.After(eventEnd) || replayEnd == eventEnd && replayBegin.Before(eventBegin) {
			return true
		}
	}
	return false
}

func (s *Sword) filterEvents(events ...*sdk.Event) ([]*sdk.Event, error) {
	result := []*sdk.Event{}
	for _, event := range events {
		// Checks if it is a replay event creation
		if !isSwordEvent(event, "replay", false) {
			continue
		}
		// Cannot modify event outside replay boundaries
		eventBegin, eventEnd, err := checkBoundaries(event, s.startTime, s.endTime)
		if err != nil {
			return result, err
		}

		if eventBegin == eventEnd || eventBegin.After(eventEnd) {
			return result, util.NewError(http.StatusBadRequest, "Invalid replay range modification")
		}
		// Checks payload error
		_, err = readPayload(event)
		if err != nil {
			return result, err
		}
		// Checks event modification
		checked, err := s.checkEventModification(event, eventBegin, eventEnd)
		if err != nil {
			return result, err
		}
		if checked {
			result = append(result, event)
			continue
		}
		if s.checkEventInsertion(event, eventBegin, eventEnd) {
			result = append(result, event)
			continue
		}

		// First event replay creation
		if len(s.replays) == 0 {
			result = append(result, event)
			continue
		}
		// If there is no valid modification or no valid creation, it's an error
		return result, util.NewError(http.StatusBadRequest, "Invalid event update")
		//return result, nil
	}
	// return replay event filtered, with no error
	return result, nil
}

func (s *Sword) modifyEvent(modified *bool, event *sdk.Event, result, replays *[]*sdk.Event) bool {
	// modifying replay event, filling all gaps
	for index, replay := range *replays {
		if event.GetUuid() == replay.GetUuid() {
			updatePayload(index, len(*replays), event)
			if event.GetBegin() != replay.GetBegin() && index > 0 {
				(*replays)[index-1].End = event.Begin
				*result = append(*result, (*replays)[index-1])
				*modified = true
			} else if event.GetEnd() != replay.GetEnd() && index < len(*replays)-1 {
				(*replays)[index+1].Begin = event.End
				*result = append(*result, (*replays)[index+1])
				*modified = true
			}
			*result = append(*result, event)
			(*replays)[index] = CloneEvent(event)
			return true
		}
	}
	return false
}

func (s *Sword) splitEvent(modified *bool, event *sdk.Event, eventBegin, eventEnd time.Time, result, replays *[]*sdk.Event) {
	// splitting and creating a new replay event, filling all gaps
	replaysCopy := []*sdk.Event{}
	size := len(*replays) + 1 // one element is added
	for index, replay := range *replays {
		replayBegin, _ := util.ParseTime(replay.GetBegin())
		replayEnd, _ := util.ParseTime(replay.GetEnd())
		if replayEnd == eventEnd && replayBegin.Before(eventBegin) {
			// the old event takes the first part of the split, the new event the second
			replay.End = event.Begin
			event.Name = replay.Name
			event.Info = replay.Info
			updatePayload(index, size, replay)
			updatePayload(index+1, size, event)
			replaysCopy = append(replaysCopy, replay)
			replaysCopy = append(replaysCopy, CloneEvent(event))
			*result = append(*result, replay)
			*result = append(*result, event)
			*modified = true
		} else if replayBegin == eventBegin && replayEnd.After(eventEnd) {
			// the old event takes the second part of the split, the new event the first
			replay.Begin = event.End
			event.Name = replay.Name
			event.Info = replay.Info
			updatePayload(index, size, event)
			updatePayload(index+1, size, replay)
			replaysCopy = append(replaysCopy, event)
			replaysCopy = append(replaysCopy, replay)
			*result = append(*result, replay)
			*result = append(*result, event)
			*modified = true
		} else {
			replaysCopy = append(replaysCopy, replay)
		}
	}
	*replays = make([]*sdk.Event, len(replaysCopy))
	copy(*replays, replaysCopy)
}

func (s *Sword) isModification(event *sdk.Event) bool {
	for _, replay := range s.replays {
		if replay.GetUuid() == event.GetUuid() {
			return true
		}
	}
	return false
}

func (s *Sword) updateReplay(events ...*sdk.Event) {
	eventsToApply := []*sdk.Event{}
	for _, event := range events {
		// check if the event has replay protocol
		if !isSwordEvent(event, "replay", false) {
			continue
		}
		// reorder events, modifications are first, insertions are last
		if s.isModification(event) {
			eventsToApply = append(eventsToApply[:0], append([]*sdk.Event{event}, events[0:]...)...)
		} else {
			eventsToApply = append(eventsToApply, event)
		}
	}
	// Apply modifications
	for _, event := range eventsToApply {
		applyModification(event, &s.replays)
	}
	return
}

func (s *Sword) deleteReplay(uuid string) {
	size := len(s.replays) - 1 // one element is deleted
	for index, replay := range s.replays {
		if replay.GetUuid() == uuid {
			if index == 0 {
				// Removing the first replay event fills the gap with the next one
				s.replays[1].Begin = replay.Begin
				updatePayload(0, size, s.replays[1])
				s.observer.UpdateEvents(s.replays[1])
			} else {
				// Removing a replay event fills the gap with the previous one
				s.replays[index-1].End = replay.End
				updatePayload(index-1, size, s.replays[index-1])
				s.observer.UpdateEvents(s.replays[index-1])
			}
			s.replays = append(s.replays[:index], s.replays[index+1:]...)
		}
	}
}

func (s *Sword) setReplayRangeDates(link *SwordLink, start, end time.Time) {
	if link == s.link && (s.startTime != start || s.endTime != end) {
		s.startTime = start
		s.endTime = end
		// remove all replay events except the first
		for i, event := range s.replays {
			if i > 0 {
				s.observer.DeleteEvent(event.GetUuid())
			}
		}
		if len(s.replays) > 0 {
			s.replays = s.replays[:1]
			ReplayRangeUuid = s.replays[0].GetUuid()
		}
		// create a new replay event
		event := &sdk.Event{
			Uuid:     proto.String(ReplayRangeUuid),
			Name:     proto.String("Replay range"),
			Begin:    proto.String(util.FormatTime(start)),
			End:      proto.String(util.FormatTime(end)),
			ReadOnly: proto.Bool(false),
			Action: &sdk.Action{
				Target:  proto.String("replay://" + s.name),
				Payload: marshal(false, false, true),
			},
		}
		s.observer.UpdateRangeDates(start, end)
		s.observer.UpdateEvents(event)
	}
}

func CloneEvent(event *sdk.Event) *sdk.Event {
	clone := &sdk.Event{}
	swapi.DeepCopy(clone, event)
	return clone
}

func (s *Sword) CheckEvents(events ...*sdk.Event) ([]*sdk.Event, bool, error) {
	modified := false
	result := []*sdk.Event{}
	// Checks
	filteredEvents, err := s.filterEvents(events...)
	if err != nil {
		return result, modified, err
	}
	replays := make([]*sdk.Event, len(s.replays))
	for i := range s.replays {
		replays[i] = CloneEvent(s.replays[i])
	}
	for _, event := range filteredEvents {
		eventBegin, eventEnd, _ := checkBoundaries(event, s.startTime, s.endTime)
		// add first replay event
		if len(replays) == 0 {
			replays = append(replays, CloneEvent(event))
			result = append(result, event)
			continue
		}
		if s.modifyEvent(&modified, event, &result, &replays) {
			continue
		}
		s.splitEvent(&modified, event, eventBegin, eventEnd, &result, &replays)
	}
	if !checkContinuity(s.startTime, s.endTime, replays) {
		return result, modified, util.NewError(http.StatusBadRequest, "Replay events are not contiguous")
	}
	return result, modified, nil
}

func (s *Sword) CheckDeleteEvent(uuid string) error {
	for _, replay := range s.replays {
		if uuid == replay.GetUuid() {
			if len(s.replays) < 2 {
				return util.NewError(http.StatusBadRequest, "Cannot remove the last replay event")
			}
			return nil
		}
	}
	return nil
}

func isActivated(event *sdk.Event) bool {
	payload, _ := readPayload(event)
	return payload.GetEnabled()
}

func (s *Sword) updateReplayTick(current time.Time) {
	skipping := false
	timeToSkip := current
	for _, event := range s.replays {
		begin, end, _ := checkBoundaries(event, s.startTime, s.endTime)
		if !skipping && current.After(begin) && current.Before(end) && !isActivated(event) {
			skipping = true
			timeToSkip = end
		} else if skipping {
			// skip to the last contiguous deactivated replay range
			if !isActivated(event) {
				timeToSkip = end
			} else {
				break
			}
		}
	}
	if skipping {
		msg := swapi.SwordMessage{
			ClientToReplay: &sword.ClientToReplay{
				Message: &sword.ClientToReplay_Content{
					ControlSkipToDate: &sword.ControlSkipToDate{
						DateTime: swapi.MakeDateTime(timeToSkip),
					},
				},
			},
		}
		s.Log("-> replay deactivated in this section, skipping to %v", timeToSkip)
		action := NewAction(gouuid.New(), url.URL{}, []byte{}, msg, false)
		s.pending[action.id] = action
		if s.status == SwordStatusConnected {
			s.link.PostAction(action)
		}
	}
}
