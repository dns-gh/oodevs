// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#ifndef __MockRolePerceiver_h_
#define __MockRolePerceiver_h_

#include "simulation_kernel/Entities/Agents/Roles/Perception/PHY_RolePion_Perceiver.h"

namespace detection
{
   class DetectionComputerFactory_ABC;//@TODO MGD Make true mock
}
// =============================================================================
/** @class  MockRolePerceiver
    @brief  MockRolePerceiver
*/
// Created: SBO 2009-06-08
// =============================================================================
class MockRolePerceiver : public mockpp::ChainableMockObject
                        , public PHY_RolePion_Perceiver
{

public:
    //! @name Constructors/Destructor
    //@{
    explicit MockRolePerceiver( )
                 : mockpp::ChainableMockObject( MOCKPP_PCHAR( "MockRolePerceiver" ) )
                 , PHY_RolePion_Perceiver( *(MIL_AgentPion*)0 )
                 , NotifyExternalPerception_mocker( "NotifyExternalPerception", this )
             {}
    virtual ~MockRolePerceiver() {}
    //@}

    //! @name Operations
    //@{
    void NotifyExternalPerception( MIL_Agent_ABC& agent, const PHY_PerceptionLevel& level )
    {
        NotifyExternalPerception_mocker.forward( agent, level );
    }
    //@}

    //! @name Member data
    //@{
    mockpp::ChainableMockMethod< void, MIL_Agent_ABC, const PHY_PerceptionLevel > NotifyExternalPerception_mocker;
    //@}
};

#endif // __MockRolePerceiver_h_
