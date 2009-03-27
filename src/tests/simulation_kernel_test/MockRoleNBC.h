// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 MASA Group
//
// *****************************************************************************

#ifndef __MockRoleNBC_h_
#define __MockRoleNBC_h_

#include "MT_Tools/MT_RoleContainer.h"
#include "simulation_kernel/Entities/Agents/Roles/NBC/PHY_RoleInterface_NBC.h"

// =============================================================================
/** @class  MockRoleNBC
    @brief  MockRoleNBC
*/
// Created: JCR 2008-09-01
// =============================================================================
class MockRoleNBC
    : public mockpp::ChainableMockObject
    , public PHY_RoleInterface_NBC
{
public:
    
    //! @name Constructors/Destructor
    //@{
    MockRoleNBC( MT_RoleContainer& container ) 
        : mockpp::ChainableMockObject( MOCKPP_PCHAR( "MockRoleNBC" ) )
        , PHY_RoleInterface_NBC( container )
        , MOCKPP_CONSTRUCT_CHAINABLE_MEMBERS( IsContaminated )
        , MOCKPP_CONSTRUCT_CHAINABLE_MEMBERS( Poison )
        , MOCKPP_CONSTRUCT_CHAINABLE_MEMBERS( Contaminate )
        , MOCKPP_CONSTRUCT_CHAINABLE_MEMBERS( Decontaminate_All )
        , MOCKPP_CONSTRUCT_CHAINABLE_MEMBERS( Decontaminate_Percent )
    {
    }
    virtual ~MockRoleNBC() {}
    //@}
        
    MOCKPP_CONST_CHAINABLE0      ( MockRoleNBC, bool, IsContaminated );

    MOCKPP_VOID_CHAINABLE1       ( MockRoleNBC, Poison, MIL_ToxicEffectManipulator );
    MOCKPP_VOID_CHAINABLE1       ( MockRoleNBC, Contaminate, MIL_ToxicEffectManipulator );
    MOCKPP_VOID_CHAINABLE_EXT0   ( MockRoleNBC, Decontaminate, _All );    
    MOCKPP_VOID_CHAINABLE_EXT1   ( MockRoleNBC, Decontaminate, MT_Float, _Percent, MT_Float );
};

#endif // __MockRoleNBC_h_
