// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 MASA Group
//
// *****************************************************************************

#ifndef __MockAgent_h_
#define __MockAgent_h_

#include "simulation_kernel/Entities/Agents/MIL_Agent_ABC.h"

// =============================================================================
/** @class  MockAgent
    @brief  MockAgent
*/
// Created: JCR 2008-09-01
// =============================================================================
MOCK_BASE_CLASS( MockAgent, MIL_Agent_ABC )
{
    explicit MockAgent( xml::xistream& xis ) : MIL_Agent_ABC( xis ) {}
    explicit MockAgent( const std::string& name = "" ) : MIL_Agent_ABC( name ) {}

    MOCK_METHOD( GetID, 0 );
    MOCK_METHOD( GetArmy, 0 );
    MOCK_METHOD( GetKnowledgeGroup, 0 );
    MOCK_METHOD( GetType, 0 );
    MOCK_METHOD( IsDead, 0 );
    MOCK_METHOD( IsMarkedForDestruction, 0 );
    MOCK_METHOD( IsNeutralized, 0 );
    MOCK_METHOD( IsPC, 0 );
    MOCK_METHOD( IsAutonomous, 0 );
    MOCK_METHOD( IsJammed, 0 );
    MOCK_METHOD( IsLogisticJammed, 0 );
    MOCK_METHOD( IsCivilian, 0 );
    MOCK_METHOD( GetCriticalIntelligence, 0 );
    MOCK_METHOD( GetAffinity, 1 );
    MOCK_METHOD( UpdateUnderIndirectFire, 0 );
    MOCK_METHOD( CanEmitReports, 0 );

//    MOCK_METHOD( GetAutomate, 0, const MIL_Automate&(), GetAutomatConstRef );
    MOCK_METHOD( GetAutomate, 0, MIL_Automate&(), GetAutomatRef );

//    MOCK_METHOD( GetDecision, 0, const DEC_Decision_ABC&(), GetDecisionConstRef );
    MOCK_METHOD( GetDecision, 0, DEC_Decision_ABC&(), GetDecisionRef );

    MOCK_METHOD( BelongsTo, 1 );

//    MOCK_METHOD( GetOrderManager, 0, const MIL_PionOrderManager&(), GetOrderManagerConstRef );
    MOCK_METHOD( GetOrderManager, 0, MIL_PionOrderManager&(), GetOrderManagerRef );

    MOCK_METHOD( ChangeSuperior, 1 );

    MOCK_METHOD( NotifyAttackedBy, 2, void( MIL_AgentPion&, bool ), NotifyAttackedByAgent );
    MOCK_METHOD( NotifyAttackedBy, 1, void( MIL_Population& ), NotifyAttackedByPopulation );

    MOCK_METHOD( MagicMove, 1 );
    MOCK_METHOD( Distance, 1 );

    MOCK_METHOD( CreateKnowledge, 1 );
    MOCK_METHOD( IsPerceived, 1 );
    MOCK_METHOD( GetKnowledge, 0 );

    MOCK_METHOD( GetLogisticHierarchy, 0 );

    MOCK_METHOD( CanInteractWithTraffic, 0 );
    MOCK_METHOD( CanBeImpactedByTraffic, 0 );
    MOCK_METHOD( InteractWithTraffic, 1, void( const std::vector< TER_Agent_ABC* >& agents ), InteractWithTrafficAgents );
    MOCK_METHOD( InteractWithTraffic, 1, void( const MIL_Agent_ABC& agent ), InteractWithTrafficAgent );

    MOCK_METHOD( CanBeDeleted, 0 )
    MOCK_METHOD( RegisterPath, 1 )
    MOCK_METHOD( UnregisterPath, 1 )
    MOCK_METHOD( HasPath, 0 )
};

#endif // __MockAgent_h_
