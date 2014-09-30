// ****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2014 MASA Group
//
// ****************************************************************************

// +build windows

package main

import (
	"fmt"
	"os"
	"os/exec"
	"path/filepath"
	"syscall"
	"unsafe"
)

var (
	advapi32         = syscall.NewLazyDLL("advapi32.dll")
	procRegCloseKey  = advapi32.NewProc("RegCloseKey")
	procRegCreateKey = advapi32.NewProc("RegCreateKeyExW")
	procRegOpenKey   = advapi32.NewProc("RegOpenKeyExW")
)

func regOpenKey(hkey uint, subkey string) bool {
	var result uint
	ret, _, _ := procRegOpenKey.Call(
		uintptr(hkey),
		uintptr(unsafe.Pointer(syscall.StringToUTF16Ptr(subkey))),
		0,
		uintptr(syscall.KEY_READ),
		uintptr(unsafe.Pointer(&result)),
	)
	if ret != 0 {
		return false
	}
	regCloseKey(result)
	return true
}

func regCloseKey(hkey uint) error {
	ret, _, _ := procRegCloseKey.Call(uintptr(hkey))
	if ret != 0 {
		return fmt.Errorf("unable to close registry key %v", hkey)
	}
	return nil
}

func regCreateKey(hkey uint, subkey string) error {
	var result uint
	ret, _, _ := procRegCreateKey.Call(
		uintptr(hkey),
		uintptr(unsafe.Pointer(syscall.StringToUTF16Ptr(subkey))),
		0, 0, 0,
		uintptr(syscall.KEY_ALL_ACCESS),
		0,
		uintptr(unsafe.Pointer(&result)),
		0,
	)
	if ret != 0 {
		return fmt.Errorf("unable to load registry key %s", subkey)
	}
	return regCloseKey(result)
}

const (
	regRoot = syscall.HKEY_CURRENT_USER
	regPath = `SOFTWARE\Masa GROUP\Sword\Replay\Redist`
)

func installVcredist(root string) error {
	installed := regOpenKey(regRoot, regPath)
	if installed {
		return nil
	}
	cmd := exec.Command(
		filepath.Join(root, "bin", "elevate.exe"),
		"-wait",
		filepath.Join(root, "bin", "vcredist.exe"),
		"/S",
		"/NCRC",
	)
	cmd.Stdout = os.Stdout
	cmd.Stderr = os.Stderr
	err := cmd.Run()
	if err != nil {
		return err
	}
	return regCreateKey(regRoot, regPath)
}
