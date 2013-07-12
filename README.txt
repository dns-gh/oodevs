gosword library contains the following modules:
- swapi: a protobuf client for Sword
- swapi/simu: helpers to configure and run a simulation and process log files
- simtests: functional tests on the simulation

To run the tests from a cygwin shell, starting at project root:

  # This step configures a golang development environment in the current shell
  $ source build/go.sh

  # Test "simtests" module using the simulation fetched by build.xml
  $ go test simtests -gocheck.f Collision

  # Reference a custom simulation executable
  $ go test simtests \
      -application d:/dev/gosword/run/vc100_x64/simulation_app.exe \
      -root-dir d:/dev/gosword/data \
      # Run only test cases matching this pattern
      -gocheck.f Collision

Note that if you test more than one module, the --test.parallel=1 option does
not seem enough to prevent them to be run concurrently. The current solution
is to adjust the resources, mostly simulation port, so *they can run in
parallel*. That is why simtests and swapi/simu tests do not share the same
port range, based on --test-port.
