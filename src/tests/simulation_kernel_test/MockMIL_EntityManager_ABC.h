// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#ifndef __MockMIL_EntityManager_ABC_h_
#define __MockMIL_EntityManager_ABC_h_

#include "Entities/MIL_EntityManager_ABC.h"

// =============================================================================
/** @class  MockMIL_EntityManager_ABC
    @brief  MockMIL_EntityManager_ABC
*/
// Created: LDC 2009-06-09
// =============================================================================
class MockMIL_EntityManager_ABC
    : public MIL_EntityManager_ABC
    , public mockpp::ChainableMockObject
{

public:
             MockMIL_EntityManager_ABC()
                : mockpp::ChainableMockObject( "MockMIL_EntityManager_ABC", 0 )
                , FindAgentPion_mocker( "FindAgentPion", this )
                , FindAutomate_mocker ( "FindAutomate", this )
                , FindObjectType_mocker( "FindObjectType", this )
             {}
    virtual ~MockMIL_EntityManager_ABC() {}
    
    virtual       MIL_AgentPion*      FindAgentPion ( unsigned int nID ) const
    {
        return FindAgentPion_mocker.forward( nID );
    }
    virtual       MIL_Automate*       FindAutomate  ( unsigned int nID ) const
    {
        return FindAutomate_mocker.forward( nID );
    }
    virtual const MIL_ObjectType_ABC& FindObjectType( const std::string& type ) const
    {
        return *FindObjectType_mocker.forward( type );
    }
    virtual MIL_KnowledgeGroup* FindKnowledgeGroup( unsigned int nID ) const
    {
        throw;
    }

    mockpp::ChainableMockMethod< MIL_AgentPion*, unsigned int > FindAgentPion_mocker;
    mockpp::ChainableMockMethod< MIL_Automate*, unsigned int > FindAutomate_mocker;
    mockpp::ChainableMockMethod< MIL_ObjectType_ABC*, std::string > FindObjectType_mocker;

};

#endif // __MockMIL_EntityManager_ABC_h_
