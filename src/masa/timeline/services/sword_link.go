// ****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2013 MASA Group
//
// ****************************************************************************
package services

import (
	"masa/sword/swapi"
	"sync"
)

type SwordLinkObserver interface {
	PostRestart(link *SwordLink, err error)
	PostLog(link *SwordLink, format string, args ...interface{})
	PostCloseAction(link *SwordLink, action string, err error)
	PostCloseWriter(link *SwordLink, writer *SwordWriter)
	PostInvalidateFilters(link *SwordLink)
}

type SwordLink struct {
	poster   SwordLinkObserver
	client   *swapi.Client             // gosword client
	listener int32                     // model listener id
	reader   *SwordReader              // message reader
	pending  sync.WaitGroup            // pending callbacks
	mutex    sync.Mutex                // protect writers
	writers  map[*SwordWriter]struct{} // pending actions
}

func NewSwordLink(poster SwordLinkObserver, address, name string, clock bool) (*SwordLink, error) {
	client, err := swapi.NewClient(address)
	if err != nil {
		return nil, err
	}
	link := &SwordLink{
		poster:  poster,
		client:  client,
		writers: map[*SwordWriter]struct{}{},
	}
	client.EnableModel = true
	client.Model.SetErrorHandler(func(d *swapi.ModelData, m *swapi.SwordMessage, err error) error {
		if err != nil {
			poster.PostLog(link, "model error: %v", err)
		}
		return nil
	})
	swapi.ConnectClient(client)
	if clock {
		link.reader = NewSwordReader(name, client)
	}
	key, err := client.GetAuthenticationKey()
	if err != nil {
		poster.PostLog(nil, "invalid key request: %v", err)
		link.Kill()
		return nil, err
	}
	err = client.LoginWithAuthenticationKey("", "", key)
	if err != nil {
		poster.PostLog(nil, "invalid login: %v", err)
		link.Kill()
		return nil, err
	}
	return link, nil
}

type SwordReaderHandler struct {
	SwordLinkObserver
	Observer
	link *SwordLink
}

func (s *SwordReaderHandler) Restart(err error) {
	s.SwordLinkObserver.PostRestart(s.link, err)
}

func (s *SwordReaderHandler) Log(format string, args ...interface{}) {
	s.SwordLinkObserver.PostLog(s.link, format, args)
}

func (s *SwordLink) Attach(observer Observer, orders Ids, actions Ids) {
	s.reader.Attach(&SwordReaderHandler{s.poster, observer, s}, orders, actions)
	s.listener = s.client.Model.RegisterListener(func(event swapi.ModelEvent) {
		s.poster.PostInvalidateFilters(s)
	})
}

func (s *SwordLink) Detach() {
	if s.reader != nil {
		s.reader.Close()
	}
	s.reader = nil
	if s.listener != 0 {
		s.client.Model.UnregisterListener(s.listener)
	}
	s.listener = 0
	s.mutex.Lock()
	defer s.mutex.Unlock()
	for writer := range s.writers {
		writer.Close()
	}
	s.writers = nil
}

func (s *SwordLink) finish() {
	// we need to wait all pending PostWithTimeout to finish
	// or suffer race-conditions inside swapi
	s.pending.Wait()
	s.client.Close()
}

func (s *SwordLink) Kill() {
	// we don't want for swapi.Client to finish
	// as it takes too long with network timeouts
	// instead, we detach it from our data
	// and kill it in the background
	s.Detach()
	if s.client != nil {
		go s.finish()
	}
}

func (s *SwordLink) CloseWriter(writer *SwordWriter) {
	// we need to protect writers because gosword
	// will close SwordWriters after a random
	// network timeout which we don't wait for
	s.mutex.Lock()
	defer s.mutex.Unlock()
	writer.Close()
	delete(s.writers, writer)
}

type SwordWriterHandler struct {
	link   *SwordLink
	writer *SwordWriter
	action string
}

func (s *SwordWriterHandler) Close() {
	s.link.poster.PostCloseWriter(s.link, s.writer)
}

func (s *SwordWriterHandler) CloseAction(err error) {
	s.link.poster.PostCloseAction(s.link, s.action, err)
}

func (s *SwordWriterHandler) Log(format string, args ...interface{}) {
	s.link.poster.PostLog(s.link, format, args...)
}

func (s *SwordLink) PostAction(action *Action) {
	s.mutex.Lock()
	defer s.mutex.Unlock()
	observer := &SwordWriterHandler{link: s, action: action.id}
	writer := NewSwordWriter(observer, action.id)
	observer.writer = writer
	s.writers[writer] = struct{}{}
	// write is done asynchronously because
	// * gosword is blocking
	// * we want to send multiple actions at once
	s.pending.Add(1)
	go func() {
		defer s.pending.Done()
		writer.Write(s.client, action)
	}()
}
