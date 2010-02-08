// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 MASA Group
//
// *****************************************************************************

#ifndef __MockAgentFactory_h_
#define __MockAgentFactory_h_

#include "simulation_kernel/AgentFactory_ABC.h"

// =============================================================================
/** @class  MockAgentFactory
    @brief  MockAgentFactory
*/
// Created: RPD 2010-02-07
// =============================================================================
class MockAgentFactory
    : public mockpp::ChainableMockObject
    , public AgentFactory_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
    explicit MockAgentFactory() 
        : mockpp::ChainableMockObject( MOCKPP_PCHAR( "MockAgentFactory" ), 0 )
        , MOCKPP_CONSTRUCT_CHAINABLE_MEMBERS( CreateAgentShadow )       
    {
    }
    virtual ~MockAgentFactory() {}
    //@}
        
    virtual MIL_AgentPion* Create( const MIL_AgentTypePion& type, MIL_Automate& automate, xml::xistream& xis )
    {
        return CreateAgentShadow();
    }

    virtual MIL_AgentPion* Create( const MIL_AgentTypePion& type, MIL_Automate& automate, const MT_Vector2D& vPosition )
    {
        return CreateAgentShadow();
    }

    MOCKPP_CONST_CHAINABLE_EXT0( MockAgentFactory, MIL_AgentPion*, CreateAgentShadow, MIL_AgentPion, );
};

#endif // __MockArmy_h_
