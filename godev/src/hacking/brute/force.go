package main

import (
	"fmt"
	"log"
)

type State struct {
	charset   string
	indexes   []int
	candidate string
	maxLength int
}

func replaceAt(str string, replacement string, index int) string {
	return str[:index] + replacement + str[index+1:]
}

func makeState(charset string, maxLength int) (*State, error) {
	if len(charset) > 0 {
		return &State{
			charset:   charset,
			candidate: charset[0:1],
			indexes:   make([]int, maxLength),
			maxLength: maxLength,
		}, nil
	}
	return nil, fmt.Errorf("charset must contain at least one character")
}

func (s *State) incremente(index int) {
	if index >= s.maxLength {
		s.candidate += "finished"
		return
	}
	if s.indexes[index]+1 == len(s.charset) {
		s.indexes[index] = 0
		s.candidate = replaceAt(s.candidate, s.charset[0:1], index)
		s.incremente(index + 1)
		return
	}
	if index >= len(s.candidate) {
		s.candidate += " "
	} else {
		s.indexes[index] += 1
	}
	at := s.indexes[index]
	s.candidate = replaceAt(s.candidate, s.charset[at:at+1], index)
}

func (s *State) first() string {
	s.candidate = s.charset[0:1]
	return s.candidate
}

func (s *State) next() string {
	s.incremente(0)
	return s.candidate
}

func Brute(maxLength int, charset string, operand func(string) bool) error {
	log.Println("running brute force...")
	state, err := makeState(charset, maxLength)
	if err != nil {
		return err
	}
	candidate := state.first()
	for {
		if len(candidate) > maxLength {
			return fmt.Errorf("brute force failed")
		}
		if operand(candidate) {
			log.Println("brute force success:", candidate)
			return nil
		}
		candidate = state.next()
	}
}
