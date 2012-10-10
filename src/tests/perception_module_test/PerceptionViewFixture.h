// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#ifndef PERCEPTION_VIEW_FIXTURE_H
#define PERCEPTION_VIEW_FIXTURE_H

#include "PerceptionFixture.h"

namespace sword
{
namespace perception
{
    struct PerceptionViewFixture : public PerceptionFixture
    {
        PerceptionViewFixture()
        {
            MOCK_EXPECT( IsInCity ).returns( false );
            MOCK_EXPECT( GetLastPostureIdentifier ).returns( 0u );
            MOCK_EXPECT( GetCurrentPostureIdentifier ).returns( 0u );
            MOCK_EXPECT( GetPostureCompletionPercentage ).returns( 1. );
            MOCK_EXPECT( ComputePerceptionDistanceFactor ).returns( 1. );
            MOCK_EXPECT( GetCollidingPopulationDensity ).returns( 1. );
        }
    };
}
}

#endif // PERCEPTION_VIEW_FIXTURE_H
