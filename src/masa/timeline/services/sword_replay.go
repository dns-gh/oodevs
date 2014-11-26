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
	"masa/timeline/sdk"
	"masa/timeline/util"
	"time"
)

var (
	ReplayRangeUuid = gouuid.New()
)

func checkTime(parameter string, replayBegin, replayEnd time.Time) (eventTime time.Time, err error) {
	eventTime, err = util.ParseTime(parameter)
	if err != nil || eventTime.Before(replayBegin) || eventTime.After(replayEnd) {
		err = ErrInvalidReplayEventParameter
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

func marshal(begin, end bool) []byte {
	buffer, _ := json.Marshal(&sdk.ReplayPayload{Begin: proto.Bool(begin), End: proto.Bool(end)})
	return buffer
}

func updatePayload(index, size int, event *sdk.Event) {
	begin := size > 1 && index != 0
	end := size > 1 && index+1 < size
	event.Action.Payload = marshal(begin, end)
}

func modifyEvent(update *sdk.Event, events []*sdk.Event) {
	index := len(events)
	updateBegin, _ := util.ParseTime(update.GetBegin())
	for i, event := range events {
		// event found and modified
		if update.Uuid == event.Uuid {
			events[i] = CloneEvent(update)
			return
		}
		begin, _ := util.ParseTime(event.GetBegin())
		end, _ := util.ParseTime(event.GetEnd())
		// find the index to insert the event
		if updateBegin.After(begin) && updateBegin.Before(end) {
			index = i
		}
	}
	// insert the event
	events = append(events[:index], append([]*sdk.Event{update}, events[index:]...)...)
}

func modifyEvents(updates, events []*sdk.Event) {
	for _, update := range updates {
		modifyEvent(update, events)
	}
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
			// Cannot modify both begin and end boundaries
			if replay.GetBegin() != event.GetBegin() && replay.GetEnd() != event.GetEnd() {
				return false, ErrInvalidReplayEventParameter
			}
			// Cannot create gap between events and replay boundaries
			if index == 0 && eventBegin.After(s.startTime) || index == len(s.replays)-1 && eventEnd.Before(s.endTime) {
				return false, ErrInvalidReplayEventParameter
			}
			// Cannot overlap more than one event
			if index > 0 {
				previousReplay, _ := util.ParseTime(s.replays[index-1].GetBegin())
				if previousReplay.After(eventBegin) {
					return false, ErrInvalidReplayEventParameter
				}
			}
			if index < len(s.replays)-1 {
				nextEnd, _ := util.ParseTime(s.replays[index+1].GetEnd())
				if nextEnd.Before(eventEnd) {
					return false, ErrInvalidReplayEventParameter
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
		// Cannot modify event with invalid range
		if eventBegin == eventEnd || eventBegin.After(eventEnd) {
			return result, ErrInvalidReplayEventParameter
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
		return result, ErrInvalidReplayEventParameter
		//return result, nil
	}
	// return replay event filtered, with no error
	return result, nil
}

func (s *Sword) modifyEvent(modified *bool, event *sdk.Event, result, replays *[]*sdk.Event) bool {
	// modifying replay event, filling all gaps
	for index, replay := range *replays {
		if event.GetUuid() == replay.GetUuid() {
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

func (s *Sword) updateReplay(event *sdk.Event) {
	// check if the event has replay protocol
	if !isSwordEvent(event, "replay", false) {
		return
	}
	// modifying replay event, filling all gaps
	for index, replay := range s.replays {
		if event.GetUuid() == replay.GetUuid() {
			s.replays[index] = CloneEvent(event)
			return
		}
	}
	s.replays = append(s.replays, CloneEvent(event))
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
				s.observer.UpdateEvent(s.replays[1].GetUuid(), s.replays[1])
			} else {
				// Removing a replay event fills the gap with the previous one
				s.replays[index-1].End = replay.End
				updatePayload(index-1, size, s.replays[index-1])
				s.observer.UpdateEvent(s.replays[index-1].GetUuid(), s.replays[index-1])
			}
			s.replays = append(s.replays[:index], s.replays[index+1:]...)
		}
	}
}

func (s *Sword) setReplayRangeDates(link *SwordLink, start, end time.Time) {
	if link == s.link {
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
				Payload: marshal(false, false),
			},
		}
		s.observer.UpdateRangeDates(start, end)
		s.observer.UpdateEvent(ReplayRangeUuid, event)
	}
}

func CloneEvent(event *sdk.Event) *sdk.Event {
	clone := &sdk.Event{}
	swapi.DeepCopy(clone, event)
	return clone
}
