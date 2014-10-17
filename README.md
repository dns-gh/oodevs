# Sword

Main sword repository

## How to build

```sh
# Use the default platform, currently vc100_x64 aka Visual Studio 2010 64*-bit
ant all
# Select a specific platform, here Visual Studio 2010 32-bit
ant -Dplatform=vc100 all
```
Temporary output files are found at out/$platform/build or build_d

Binaries are generated into out/$platform/release or debug

They need to have run/$platform in the PATH to work

## Generating Visual Studio projects
Visual studio project files are generated from build/CMakeLists.txt with this target
```sh
ant cmake.msvc
```
A solution will be generated at out/$platform/msvc/sword.sln

There are a few cmake limitations:
* You need to set project working directory to run/$platform manually on debug applications
* You need to close visual-studio and run ```ant cmake.msvc``` again, when adding or removing files.

## C++ binaries
```sh
# Build all binaries & libraries
ant build
# Build only simulation_app
ant build -Dninja.target=simulation_app
# Control how many parallel jobs are used (default is 4)
ant build -Dninja.jobs=8
```
- **simulation_app**: Sword simulation server, instanciate & run a multiplayer exercise
- **gaming_app**:  Sword gaming client, connects to a server and dislay the tactical view
- **preparation_app**: Sword authoring application, create & modify exercises
- **adaptation_app**: Sword adaptation application, create & modify the physical model
- **selftraining_app**: Frontend to create, modify & run exercises
- **replayer_app**: Sword replay server, replay an exercise from saved records
- **cloud_server**: Sword admin service, manage sword exercises through a web interface
- **cloud_client**: Sword admin client, download an exercise from an admin server and
start gaming locally

## Go binaries
All go binaries and libraries are built in one target with
```sh
ant go
```

- masa/sword/cmd/**csvclient**: an application to export logistic data from a simulation under csv
  format
- masa/sword/cmd/**dumpreplay**: utility to extract and print information from replay data
- masa/sword/cmd/**exdec**: utility to inject decisional code in the simulation
- masa/sword/cmd/**stressclient**: spam a simulation with crafted messages and check their replies
- masa/sword/cmd/**testclient**: a simple client creating an swapi.Client instance, connecting it
- masa/sword/**simtests**: functional tests on the simulation
- masa/sword/**swapi**: a protobuf client for Sword
- masa/sword/swapi/**phy**: helpers to read physical data
- masa/sword/swapi/**replay**: helpers to parse replay data
- masa/sword/swapi/**simu**: helpers to configure and run a simulation and process log files
- masa/sword/**sword**: auto generated protobuf library from sword .proto files
  to a running simulation and wait for connection termination. Use it to test
  the model behaviour against real world scenarii.
- masa/**testproto**: test goprotobuf behaviour with regard to .proto changes
- masa/**timeline**: Timeline server, player and utilities
- masa/**timelineui**: Timeline ui test.

## How to run tests

There are two types of tests, unit test and integration tests.

### Unit tests

Unit tests are in c++ and found in src/tests. They can be run with
```sh
# Run all unit tests
ant unit_test
# Run protocol_test only
ant unit_test -Dctest.tests=protocol_test
```
When using Visual Studio, those tests are automatically run when building the test projects

Note that those tests tend to be deprecated, in favor of integration tests

### Integration tests

Integration tests are found in src/masa/sword/simtests. They can be run with
```sh
# Run every go tests
ant go.test
# Run Collision tests only
ant go.test -Dgocheck.f=Collision
```
You need to build simulation_app, replayer_app & gaming_app before

You can also run tests manually, from a cygwin shell
```sh
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
```

Note that if you test more than one module, the --test.parallel=1 option does
not seem enough to prevent them to be run concurrently. The current solution
is to adjust the resources, mostly simulation port, so *they can run in
parallel*. That is why simtests and swapi/simu tests do not share the same
port range, based on --test-port.

## How to use the admin server
It is possible to run an admin server from freshly compiled sources
```sh
# From scratch
ant admin.all admin.run
# Recompile dependencies and run the server
ant admin.deploy admin.run
# Start the admin server only
ant admin.run
```
Due to packaging limitations, some links will be missing, like the sword 32-bit cloud client setup

You also need to upload packages each time you reset the out/$platform/admin directory

If you need to debug the application under visual studio, the easiest solution is to run  ```ant admin.deploy admin.run``` once, copy the command-line parameters from cloud_server.exe, and use it as command line arguments on the cloud_server debug project. Do not forget to also set the working directory to run/$platform

## How to use the timeline server
The timeline server and its dependencies are built with
```sh
ant timeline
```
This target will compile & minify coffeescript files into javascript, compile less files into css, and test the various components.

When developing new web features, the easiest way is
- Build the timeline server, selftraining_app, simulation_app & gaming_app
- Start selftraining_app with the --debug flag
- Set the timeline debug directory to src/masa/timeline/www, so the server use non-minified javascript
- Start build/watch_timeline.cmd, which will rebuild timeline www components periodically
- Start editing coffeescript files, and view result & content on your prefered browser
