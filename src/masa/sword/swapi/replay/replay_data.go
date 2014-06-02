// ****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2013 MASA Group
//
// ****************************************************************************

// Package replay implements structures to read replay data.
//
// Replay data looks like:
//
// session / record / 00000000 / key
//                             / keyindex
//                             / index
//                             / update
//                             / info
//                  / ...
//                  / current  / ...
//
// Under the "record" directory are "fragments" which contain information for
// a tick range described in the "info" file. The (keyindex, key) and
// (index, update) pairs contain messages, the index file is a fixed size
// record index pointing to blocks in the data file. Data blocks are sequences
// of one or more protobuf messages prefixed by size.
//
// The "key" pair describes key frames data blocks. Each message sequence
// describes a full simulation state at a given tick.
//
// The "update" pair describes delta frames. Each message sequence is a sequence
// of update message as sent by the simulation while operating.
package replay

import (
	"code.google.com/p/goprotobuf/proto"
	"encoding/binary"
	"fmt"
	"io"
	"io/ioutil"
	"masa/sword/sword"
	"os"
	"path/filepath"
)

// List replay segment directories of a session.
func ListSegmentDirs(recordsDir string) ([]string, error) {
	if _, err := os.Stat(filepath.Join(recordsDir, "00000000")); err != nil {
		return nil, fmt.Errorf("%s does not look like a replay directory", recordsDir)
	}
	entries, err := ioutil.ReadDir(recordsDir)
	if err != nil {
		return nil, err
	}
	dirs := []string{}
	for _, e := range entries {
		if e.IsDir() {
			dirs = append(dirs, filepath.Join(recordsDir, e.Name()))
		}
	}
	return dirs, err
}

// fileIndex is a file-based fixed sized records contained.
type fileIndex struct {
	fp         *os.File
	recordSize uint32
	size       uint32
}

func newFileIndex(path string, recordSize uint32) (*fileIndex, error) {
	fp, err := os.Open(path)
	if err != nil {
		return nil, err
	}
	fileSize, err := fp.Seek(0, 2)
	if err != nil {
		fp.Close()
		return nil, err
	}
	return &fileIndex{
		fp:         fp,
		recordSize: recordSize,
		size:       uint32(fileSize) / recordSize,
	}, nil
}

func (f *fileIndex) Close() error {
	return f.fp.Close()
}

// Returns fileIndex records count.
func (f *fileIndex) Size() uint32 {
	return f.size
}

// Read "index" record and unpack it as supplied data using binary.Read.
func (f *fileIndex) GetData(index uint32, data interface{}) error {
	if index > f.size {
		return fmt.Errorf("index offset out of bounds")
	}
	offset := index * f.recordSize
	_, err := f.fp.Seek(int64(offset), 0)
	if err != nil {
		return err
	}
	err = binary.Read(f.fp, binary.LittleEndian, data)
	return nil
}

// Use ReplayData to extract messages stored in "update" or "key" data files.
type ReplayData struct {
	fp *os.File
}

func NewReplayData(path string) (*ReplayData, error) {
	fp, err := os.Open(path)
	if err != nil {
		return nil, err
	}
	return &ReplayData{
		fp: fp,
	}, nil
}

func (r *ReplayData) Close() error {
	return r.fp.Close()
}

// Returns the message sequence stored in a data block.
func (r *ReplayData) Read(offset, size uint32) ([]*sword.SimToClient, error) {
	messages := []*sword.SimToClient{}
	if size == 0 {
		return messages, nil
	}
	_, err := r.fp.Seek(int64(offset), 0)
	if err != nil {
		return nil, err
	}
	read := uint32(0)
	for read < size {
		dataSize := uint32(0)
		err = binary.Read(r.fp, binary.LittleEndian, &dataSize)
		if err != nil {
			return nil, err
		}
		buffer := make([]byte, dataSize)
		_, err = io.ReadFull(r.fp, buffer)
		if err != nil {
			return nil, err
		}
		msg := &sword.SimToClient{}
		err = proto.Unmarshal(buffer, msg)
		messages = append(messages, msg)
		read += dataSize + 4
	}
	return messages, err
}

type Frame struct {
	Offset uint32
	Size   uint32
}

// Use UpdateIndex to access "index" file.
type UpdateIndex struct {
	*fileIndex
}

func NewUpdateIndex(path string) (*UpdateIndex, error) {
	index, err := newFileIndex(path, uint32(binary.Size(&Frame{})))
	if err != nil {
		return nil, err
	}
	return &UpdateIndex{index}, nil
}

func (r *UpdateIndex) GetFrame(index uint32) (Frame, error) {
	frame := Frame{}
	err := r.fileIndex.GetData(index, &frame)
	return frame, err
}

func enumerateSegmentUpdates(segmentDir string,
	handler func(Frame, *sword.SimToClient) error) error {

	// Open index and data file
	indexPath := filepath.Join(segmentDir, "index")
	if _, err := os.Stat(indexPath); os.IsNotExist(err) {
		// current may be empty, ignore it
		return nil
	}
	index, err := NewUpdateIndex(indexPath)
	if err != nil {
		return err
	}
	defer index.Close()
	data, err := NewReplayData(filepath.Join(segmentDir, "update"))
	if err != nil {
		return err
	}
	defer data.Close()

	for i := uint32(0); i != index.Size(); i++ {
		f, err := index.GetFrame(i)
		if f.Size <= 0 {
			// Trailing records are sometime empty, ignore them
			continue
		}
		msgs, err := data.Read(f.Offset, f.Size)
		if err != nil {
			return err
		}
		for _, msg := range msgs {
			err = handler(f, msg)
			if err != nil {
				return err
			}
		}
	}
	return nil
}

// Invoke supplied handler on all messages extracted from session replay update
// data. Errors returned by the handler terminates the enumeration.
func EnumerateReplayUpdates(recordsDir string,
	handler func(Frame, *sword.SimToClient) error) error {

	dirs, err := ListSegmentDirs(recordsDir)
	if err != nil {
		return err
	}
	for _, dir := range dirs {
		err = enumerateSegmentUpdates(dir, handler)
		if err != nil {
			return err
		}
	}
	return nil
}

type KeyFrame struct {
	FrameNum int32
	Offset   int32
	Size     int32
}

// Use KeyFrameIndex to access "keyindex" file.
type KeyFrameIndex struct {
	*fileIndex
}

func NewKeyFrameIndex(path string) (*KeyFrameIndex, error) {
	index, err := newFileIndex(path, uint32(binary.Size(&KeyFrame{})))
	if err != nil {
		return nil, err
	}
	return &KeyFrameIndex{index}, nil
}

func (k *KeyFrameIndex) GetKeyFrame(index uint32) (KeyFrame, error) {
	keyFrame := KeyFrame{}
	err := k.fileIndex.GetData(index, &keyFrame)
	return keyFrame, err
}

func enumerateSegmentKeyFrames(segmentDir string,
	handler func(KeyFrame, *sword.SimToClient) error) error {

	// Open index and data file
	indexPath := filepath.Join(segmentDir, "keyindex")
	if _, err := os.Stat(indexPath); os.IsNotExist(err) {
		// current may be empty, ignore it
		return nil
	}
	index, err := NewKeyFrameIndex(indexPath)
	if err != nil {
		return err
	}
	defer index.Close()
	data, err := NewReplayData(filepath.Join(segmentDir, "key"))
	if err != nil {
		return err
	}
	defer data.Close()

	for i := uint32(0); i != index.Size(); i++ {
		f, err := index.GetKeyFrame(i)
		if f.Size <= 0 {
			// Trailing records are sometime empty, ignore them
			continue
		}
		msgs, err := data.Read(uint32(f.Offset), uint32(f.Size))
		if err != nil {
			return err
		}
		for _, msg := range msgs {
			err = handler(f, msg)
			if err != nil {
				return err
			}
		}
	}
	return nil
}

func EnumerateReplayKeyFrames(recordsDir string,
	handler func(KeyFrame, *sword.SimToClient) error) error {

	dirs, err := ListSegmentDirs(recordsDir)
	if err != nil {
		return err
	}
	for _, dir := range dirs {
		err := enumerateSegmentKeyFrames(dir, handler)
		if err != nil {
			return err
		}
	}
	return nil
}
