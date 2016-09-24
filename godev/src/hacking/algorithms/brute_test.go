package algorithms

import (
	"testing"
)

const (
	alphabetical = "abcdefghijklmnopqrstuvwxyz"
)

func launchTestBruteForce(toFind string, cpu int) error {
	length := len(toFind)
	return BruteForce(length, cpu, alphabetical, func(candidate string) bool {
		return candidate == toFind
	})
}

func TestBruteForce(t *testing.T) {
	err := launchTestBruteForce("test", 1)
	if err != nil {
		t.Error("Test failed:", err.Error())
	}
}

func benchBruteForce(b *testing.B, cpu int) {
	for n := 0; n < b.N; n++ {
		launchTestBruteForce("zzzzz", cpu)
	}
}

func BenchmarkBruteForceCPU1(b *testing.B) { benchBruteForce(b, 1) }
func BenchmarkBruteForceCPU2(b *testing.B) { benchBruteForce(b, 2) }
func BenchmarkBruteForceCPU3(b *testing.B) { benchBruteForce(b, 3) }
func BenchmarkBruteForceCPU4(b *testing.B) { benchBruteForce(b, 4) }
