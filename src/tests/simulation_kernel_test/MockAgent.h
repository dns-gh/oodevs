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

class DEC_Decision_ABC;

// =============================================================================
/** @class  MockAgent
    @brief  MockAgent
*/
// Created: JCR 2008-09-01
// =============================================================================
class MockAgent
    : public mockpp::ChainableMockObject
    , public MIL_Agent_ABC
{
public:
    
    //! @name Constructors/Destructor
    //@{
    MockAgent() 
        : mockpp::ChainableMockObject( MOCKPP_PCHAR( "MockAgent" ) )
        , MOCKPP_CONSTRUCT_CHAINABLE_MEMBERS( GetID )
        , MOCKPP_CONSTRUCT_CHAINABLE_MEMBERS( IsDead )
        , MOCKPP_CONSTRUCT_CHAINABLE_MEMBERS( IsNeutralized )
        , MOCKPP_CONSTRUCT_CHAINABLE_MEMBERS( IsPC )
        , MOCKPP_CONSTRUCT_CHAINABLE_MEMBERS( GetTypeShadow )        
        , MOCKPP_CONSTRUCT_CHAINABLE_MEMBERS( GetArmyShadow )
        , MOCKPP_CONSTRUCT_CHAINABLE_MEMBERS( NotifyAttackedBy_Pion )
        , MOCKPP_CONSTRUCT_CHAINABLE_MEMBERS( NotifyAttackedBy_Population )
        , MOCKPP_CONSTRUCT_CHAINABLE_MEMBERS( BelongsTo )
        , MOCKPP_CONSTRUCT_CHAINABLE_MEMBERS( IsPerceived )
        , MOCKPP_CONSTRUCT_CHAINABLE_MEMBERS( CreateKnowledgeShadow )
    {
    }
    virtual ~MockAgent() {}
    //@}
        
    virtual const MIL_AgentType_ABC&  GetType() const
    {
        return *GetTypeShadow();
    }

    virtual MIL_Army_ABC& GetArmy() const
    {
        return *GetArmyShadow();
    }

    virtual DEC_Knowledge_Agent& CreateKnowledge( const MIL_KnowledgeGroup& knowledgeGroup )
    {
        return *CreateKnowledgeShadow( knowledgeGroup );
    }
    virtual DEC_KnowledgeBlackBoard_AgentPion& GetKnowledge   () const
    {
        throw;
    }
    
    virtual       DEC_Decision_ABC& GetDecision   () { throw; }
    virtual const DEC_Decision_ABC& GetDecision   () const { throw; }

    MOCKPP_CONST_CHAINABLE0          ( MockAgent, uint, GetID );
    MOCKPP_CONST_CHAINABLE0          ( MockAgent, bool, IsDead );
    MOCKPP_CONST_CHAINABLE0          ( MockAgent, bool, IsNeutralized );
    MOCKPP_CONST_CHAINABLE0          ( MockAgent, bool, IsPC );

    MOCKPP_CONST_CHAINABLE_EXT0      ( MockAgent, const MIL_AgentType_ABC*, GetTypeShadow, MIL_AgentType_ABC, );   
    MOCKPP_CONST_CHAINABLE_EXT0      ( MockAgent, MIL_Army_ABC*, GetArmyShadow, MIL_Army_ABC, );   

    MOCKPP_VOID_CHAINABLE_EXT1       ( MockAgent, NotifyAttackedBy, MIL_AgentPion&, _Pion, MIL_AgentPion );
    MOCKPP_VOID_CHAINABLE_EXT1       ( MockAgent, NotifyAttackedBy, MIL_Population&, _Population, MIL_Population );

    MOCKPP_CONST_CHAINABLE_EXT1      ( MockAgent, bool, BelongsTo, const MIL_KnowledgeGroup&, bool, , MIL_KnowledgeGroup );
    MOCKPP_CONST_CHAINABLE_EXT1      ( MockAgent, bool, IsPerceived, const MIL_Agent_ABC&, bool, , MIL_Agent_ABC );
    MOCKPP_CHAINABLE_EXT1            ( MockAgent, DEC_Knowledge_Agent*, CreateKnowledgeShadow, const MIL_KnowledgeGroup&, DEC_Knowledge_Agent, , MIL_KnowledgeGroup );    
};

#endif // __MockAgent_h_
