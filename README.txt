go sword library contains the following modules:
- masa/sword/cmd/csvclient: an application to export logistic data from a simulation under csv
  format
- masa/sword/cmd/dumpreplay: utility to extract and print information from replay data
- masa/sword/cmd/exdec: utility to inject decisional code in the simulation
- masa/sword/cmd/stressclient: spam a simulation with crafted messages and check their replies
- masa/sword/cmd/testclient: a simple client creating an swapi.Client instance, connecting it
- masa/sword/simtests: functional tests on the simulation
- masa/sword/swapi: a protobuf client for Sword
- masa/sword/swapi/phy: helpers to read physical data
- masa/sword/swapi/replay: helpers to parse replay data
- masa/sword/swapi/simu: helpers to configure and run a simulation and process log files
- masa/sword/sword: auto generated protobuf library from sword .proto files
  to a running simulation and wait for connection termination. Use it to test
  the model behaviour against real world scenarii.
- masa/testproto: test goprotobuf behaviour with regard to .proto changes
- masa/timeline: Timeline server, playe rand utilities
- masa/timelineui: Timeline ui test.

To run the tests from a cygwin shell, starting at project root:

  # This step configures a golang development environment in the current shell
  $ source build/go.sh

  # Test "masa/sword/simtests" module using the simulation fetched by build.xml
  $ go test masa/sword/simtests -gocheck.f Collision
  # Alternately, go test ...simtests -gocheck.f Collision

  # Reference a custom simulation executable
  $ go test masa/sword/simtests \
      -application d:/dev/gosword/run/vc100_x64/simulation_app.exe \
      -root-dir d:/dev/gosword/data \
      # Run only test cases matching this pattern
      -gocheck.f Collision

Note that if you test more than one module, the --test.parallel=1 option does
not seem enough to prevent them to be run concurrently. The current solution
is to adjust the resources, mostly simulation port, so *they can run in
parallel*. That is why simtests and swapi/simu tests do not share the same
port range, based on --test-port.
