// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "integration_decisionnal_test_pch.h"
#include <directia/brain/Brain.h>
#include <boost/bind.hpp>
#include <boost/function.hpp>

namespace
{
    class BrainFixture
    {
    public:
        BrainFixture()
        : brain( BRAIN_INIT )
        {
            brain[ "include" ]( std::string("integration/ToolsFunctions.lua") );
        }
        void LinearInterpolationTest( double min, double max, double start, double stop, bool upslop, double value, double expected )
        {
            directia::tools::binders::ScriptRef linearInterpolation = brain[ "LinearInterpolation" ];
            BOOST_CHECK_CLOSE( linearInterpolation.Call< double >( min, max, start, stop, upslop, value ), expected, std::numeric_limits<float>::epsilon() );
        }
    private:
        directia::brain::Brain brain;
    };
}
// -----------------------------------------------------------------------------
// Name: LinearInterpolation
// Created: MGD 2010-01-22
// -----------------------------------------------------------------------------
BOOST_FIXTURE_TEST_CASE( LinearInterpolation, BrainFixture )
{
    LinearInterpolationTest( 0., 1., 0., 1. , true, 0.5, 0.5 );
    LinearInterpolationTest( 0.3, 0.7, 1., 2. , true, 0., 0.3 );
    LinearInterpolationTest( 0.3, 0.7, 1., 2. , true, 3., 0.7 );
    LinearInterpolationTest( 0.3, 0.7, 1., 2. , true, 1.25, 0.4 );
    LinearInterpolationTest( 0.3, 0.7, 1., 2. , false, 1.25, 0.6 );
}
