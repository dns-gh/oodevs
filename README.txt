gosword library contains the following modules:
- swapi: a protobuf client for Sword
- swapi/simu: helpers to configure and run a simulation and process log files
- simtests: functional tests on the simulation

"simtests" contains regular Go tests, runnable with "go test". Most of them
require additional command line options to get a simulation executable and
a root directory.

To run the tests from a cygwin shell, starting at project root:

  # This step configures a golang development environment in the current shell
  $ source build/go.sh

  # Test "simtests" module
  $ go test simtests \
      --applications d:/dev/gosword/run/vc100_x64/simulation_app.exe \
      --root-dir d:/dev/gosword/data \
      # Run only test cases matching this pattern
      --test.run '.*Collision.*'
      # Do not run the tests in parallel
      --test.parallel=1

Note that if you test more than one module, the --test.parallel=1 option does
not seem enough to prevent them to be run concurrently. The current solution
is to adjust the resources, mostly simulation port, so *they can run in
parallel*. That is why simtests and swapi/simu tests do not share the same
port range, based on --test-port.
