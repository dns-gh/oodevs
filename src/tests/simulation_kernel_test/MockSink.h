// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#ifndef MockSink_h
#define MockSink_h

#include "simulation_kernel/Adapters/Sink_ABC.h"
#include "propagation/FloodModel_ABC.h"

// =============================================================================
/** @class  MockSink
    @brief  Mock sink
*/
// Created: LGY 2012-09-20
// =============================================================================
MOCK_BASE_CLASS( MockSink, sword::Sink_ABC )
{
    MOCK_METHOD( Finalize, 0 );
    MOCK_METHOD( Clean, 0 );
    MOCK_METHOD( ExecutePerceptions, 0 );
    MOCK_METHOD( ExecuteCommands, 0 );
    MOCK_METHOD( ApplyEffects, 0 );
    MOCK_METHOD( NotifyEffects, 0 );
    MOCK_METHOD( UpdateModel, 4 );
    MOCK_METHOD( UpdateKnowledges, 0 );
    MOCK_METHOD( UpdateUrbanModel, 1 );
    MOCK_METHOD( LogProfiling, 0 );
    MOCK_METHOD( GetModelCount, 0 );

    MOCK_METHOD( Create, 4, MIL_AgentPion*( const MIL_AgentTypePion&, MIL_Automate&, xml::xistream&, sword::RoleExtender_ABC* ), CreateFromXml );
    MOCK_METHOD( Create, 5, MIL_AgentPion*( const MIL_AgentTypePion&, MIL_Automate&, const MT_Vector2D&, const std::string*, sword::RoleExtender_ABC* ), CreateFromPointAndName );

    MOCK_METHOD( StartCommand, 2 );
    MOCK_METHOD( StopCommand, 1 );

    MOCK_METHOD( CreateFloodModel, 0 );
};

#endif // MockSink_h
