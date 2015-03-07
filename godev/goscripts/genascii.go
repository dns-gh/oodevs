package main

import (
	"flag"
	"fmt"
	"math"
	"bufio"
	"os"
)

// TODO
// add generation of nodata values of different types : bands, noise, holes
// add filepath management/option/flag
// add flags relative to exported fields : xllcorner, yllcorner, CELLSIZE and NODATA_VALUE

func main() {

	//relativeFilePathPtr := flag.String("rpath", "", "relative file path to the test dir")
	stepPtr := flag.Float64("step", math.Pi/2, "step of the grid")
	freqPtr := flag.Float64("freq", 1.0, "sinusoidal frequency at the begining of the generation")
	freqEndPtr := flag.Float64("lastfreq", 1.0, "sinusoidal frequency at the end of the generation")
	//randNoisePtr := flag.Float64("noise", 0.0, "randomn noise from 0 to 1 (completly noised)")
	
	flag.Parse()
	fmt.Println("Generating ASCII grid...")
	step := *stepPtr
	freq := *freqPtr
	freqEnd := * freqEndPtr
	fmt.Println("step : ", step)
	fmt.Println("freq : ", freq)

	tabSize := int(2.0*math.Pi/step)
	dataTab := make([][]float64, 1, 1000)

	valueTab := make([]float64, 0, 1000)
	for i := 0; i < tabSize; i++ {
		valueTab = append(valueTab, step * float64(i))
	}

	// linear interpolation between the first frequence and the last
	linearPoly := make([]float64, 0, 1000)
	for i := 0; i < tabSize; i++ {
		linearPoly = append(linearPoly, float64(i)/float64(tabSize - 1))
	}

	// fill the grid
	for i := 0; i < tabSize; i++ {
		dataTab[i] = make([]float64, 0)
		for j := 0; j < tabSize; j++ {
			dataTab[i] = append(dataTab[i], 500 * math.Sin( valueTab[i] * (linearPoly[i]*freqEnd + (1 - linearPoly[i])*freq ) ) + 500 )
		}
		dataTab = append(dataTab, make([]float64, 0))
	}

	// create file to save results in ascii format
	f, err := os.Create("sinusGrid.asc")
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