// ****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2013 MASA Group
//
// ****************************************************************************
package main

import (
	"fmt"
	"masa/sword/swapi"
	"masa/sword/swapi/simu"
	"masa/sword/swrun"
	"masa/sword/swtest"
	"os"
)

var (
	Cfg *swtest.Config
)

func NewAdminOpts(exercise string) *swrun.ClientOpts {
	return &swrun.ClientOpts{
		Exercise: exercise,
		User:     "admin",
		Password: "",
	}
}

func connectAndWaitModel(opts *swrun.ClientOpts) (*simu.SimProcess, *swapi.Client, error) {
	return swrun.ConnectAndWaitModel(opts, Cfg)
}

func BenchmarkEmptyRoute() ([]float64, error) {
	sim, client, err := connectAndWaitModel(NewAdminOpts("crossroad-small-empty"))
	if err != nil {
		return nil, err
	}
	defer swrun.StopSimAndClient(sim, client)
	return []float64{1}, nil
}

func benchmark() error {
	Cfg = swtest.ParseFlags()

	// Assume benchmarks are run from sword project root directory
	cwd, err := os.Getwd()
	if err != nil {
		return err
	}
	Cfg.BaseDir = cwd

	res, err := BenchmarkEmptyRoute()
	fmt.Println(res)
	return err
}

func main() {
	err := benchmark()
	if err != nil {
		fmt.Fprintf(os.Stderr, "error: %s\n", err)
		os.Exit(1)
	}
}
