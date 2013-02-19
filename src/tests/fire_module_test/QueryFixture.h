// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#ifndef QUERY_FIXTURE_H
#define QUERY_FIXTURE_H

#include "ModuleFixture.h"

namespace
{
    MOCK_FUNCTION( filter, 1, bool( const SWORD_Model* component ), filter )

    struct TargetFixture : sword::fire::ModuleFixture
    {
        TargetFixture()
            : target( model[ "knowledges" ][ 1242 ][ "agents" ][ 51 ] )
            , enemy ( core::Convert( &target ) )
        {
            target[ "components" ];
        }
        core::Model& target;
        SWORD_Model* enemy;
    };

    struct QueryFixture : TargetFixture
    {
        ~QueryFixture()
        {
            MOCK_VERIFY( filter );
            MOCK_RESET( filter );
        }
    };

    struct EnemyFixture : QueryFixture
    {
        EnemyFixture()
            : component_2( target[ "components" ].AddElement() )
        {
            component_2[ "volume" ] = volume_1;
            target[ "major" ] = 0;
        }
        core::Model& component_2;
    };
}

#endif // QUERY_FIXTURE_H
