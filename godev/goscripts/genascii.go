package main

import (
	"flag"
	"fmt"
	"math"
	"bufio"
	"os"
)

func main() {

	//relativeFilePathPtr := flag.String("rpath", "", "relative file path to the test dir")
	stepPtr := flag.Float64("step", 0.002, "step of the grid")
	freqPtr := flag.Float64("freq", 1.0, "sinusoidal frequency")
	//randNoisePtr := flag.Float64("noise", 0.0, "randomn noise from 0 to 1 (completly noised)")
	
	flag.Parse()
	fmt.Println("Generating ASCII grid...")
	step := *stepPtr
	freq := *freqPtr
	fmt.Println("step : ", step)
	fmt.Println("freq : ", freq)

	tabSize := int(2.0*math.Pi/step)
	dataTab := make([][]float64, 1, 1000)

	valueTab := make([]float64, 0, 1000)
	for i := 0; i < tabSize; i++ {
		valueTab = append(valueTab, step * float64(i))
	}

	for i := 0; i < tabSize; i++ {
		dataTab[i] = make([]float64, 0)
		for j := 0; j < tabSize; j++ {
			dataTab[i] = append(dataTab[i], 500 * math.Sin( valueTab[i] * freq ) + 500 )
		}
		dataTab = append(dataTab, make([]float64, 0))
	}

	f, err := os.Create("data/tests/testdata/sinus-multiple-freq/asciigrid/sinusGrid.asc")
    check(err)
    defer f.Close()

	w := bufio.NewWriter(f)

	fmt.Println("ncols : ", tabSize)
	fmt.Println("nrows : ", tabSize)
    fmt.Fprintf(w, "ncols %d\n", tabSize)
    fmt.Fprintf(w, "nrows %d\n", tabSize)
	fmt.Fprintf(w, "xllcorner %d\n", 0)
    fmt.Fprintf(w, "yllcorner %d\n", 0)
    fmt.Fprintf(w, "CELLSIZE %d\n", 100)
    fmt.Fprintf(w, "NODATA_VALUE %d\n", -30000)

    for i := 0; i < tabSize; i++ {
		for j := 0; j < tabSize; j++ {
			fmt.Fprintf(w, "%6.2f ", dataTab[i][j])
		}
		fmt.Fprintf(w, "\n")
	}

	w.Flush()

	fmt.Println("ASCII grid generated...")
}

func check(e error) {
    if e != nil {
        panic(e)
    }
}