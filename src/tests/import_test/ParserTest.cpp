// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#include "import_test_pch.h"
#include "import/Parser.h"
#include <boost/filesystem.hpp>

namespace bfs = boost::filesystem;

// -----------------------------------------------------------------------------
// Name: ImportGeneratesSomeFiles
// Created: MCO 2010-07-16
// -----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE( ImportGeneratesSomeFiles )
{
    const bfs::path outdir = "./import_test";
    bfs::remove_all( outdir );
    Parser( "../../data/tests/import_lto/scenarioExemple.xml", "../../data", "/tests/import_lto/exercise.xml", outdir.string() ).Generate();
    BOOST_CHECK( bfs::exists( outdir ) );
    BOOST_CHECK( bfs::exists( outdir / "orbat.xml" ) );
    BOOST_CHECK( bfs::exists( outdir / "traduction.log" ) );
    BOOST_CHECK( bfs::exists( outdir / "weather.xml" ) );
    BOOST_CHECK( bfs::exists( outdir / "Temps de manoeuvre 1.ord" ) );
    BOOST_CHECK( bfs::exists( outdir / "TM2.ord" ) );
    bfs::remove_all( outdir );
}

// -----------------------------------------------------------------------------
// Name: ImportGeneratesSomeFiles2
// Created: MCO 2010-07-16
// -----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE( ImportGeneratesSomeFiles2 )
{
    const bfs::path outdir = "./import_test2";
    bfs::remove_all( outdir );
    Parser( "../../data/tests/import_lto/Test_pour_import_OEC.xml", "../../data", "/tests/import_lto/exercise.xml", outdir.string() ).Generate();
    BOOST_CHECK( bfs::exists( outdir ) );
    BOOST_CHECK( bfs::exists( outdir / "orbat.xml" ) );
    BOOST_CHECK( bfs::exists( outdir / "traduction.log" ) );
    BOOST_CHECK( bfs::exists( outdir / "weather.xml" ) );
    BOOST_CHECK( bfs::exists( outdir / "Début des mouvements.ord" ) );
    BOOST_CHECK( bfs::exists( outdir / "suite et Fin.ord" ) );
    bfs::remove_all( outdir );
}
