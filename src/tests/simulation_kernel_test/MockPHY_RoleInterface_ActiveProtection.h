// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#ifndef __MockPHY_RoleInterface_ActiveProtection_h_
#define __MockPHY_RoleInterface_ActiveProtection_h_

#include "Entities/Agents/Roles/Protection/PHY_RoleInterface_ActiveProtection.h"

// =============================================================================
/** @class  MockPHY_RoleInterface_ActiveProtection
    @brief  MockPHY_RoleInterface_ActiveProtection
*/
// Created: LDC 2010-01-07
// =============================================================================
class MockPHY_RoleInterface_ActiveProtection
    : public mockpp::ChainableMockObject
    , public PHY_RoleInterface_ActiveProtection
{

public:
    //! @name Constructors/Destructor
    //@{
    MockPHY_RoleInterface_ActiveProtection()
        : mockpp::ChainableMockObject( MOCKPP_PCHAR( "MockPHY_RoleInterface_ActiveProtection" ), 0 )
        , MOCKPP_CONSTRUCT_CHAINABLE_MEMBERS( UseAmmunition )
        , MOCKPP_CONSTRUCT_CHAINABLE_MEMBERS( GetPHModifier )
        , MOCKPP_CONSTRUCT_CHAINABLE_MEMBERS( CounterIndirectFire )
        , MOCKPP_CONSTRUCT_CHAINABLE_MEMBERS( DestroyIndirectFire )
        {}
    virtual ~MockPHY_RoleInterface_ActiveProtection() {}
    //@}

    MOCKPP_VOID_CHAINABLE1( MockPHY_RoleInterface_ActiveProtection, UseAmmunition, PHY_DotationCategory );
    MOCKPP_CONST_CHAINABLE1( MockPHY_RoleInterface_ActiveProtection, double, GetPHModifier, PHY_DotationCategory );
    MOCKPP_CONST_CHAINABLE1( MockPHY_RoleInterface_ActiveProtection, bool, CounterIndirectFire, PHY_DotationCategory );
    MOCKPP_CONST_CHAINABLE1( MockPHY_RoleInterface_ActiveProtection, bool, DestroyIndirectFire, PHY_DotationCategory );

};


#endif // __MockPHY_RoleInterface_ActiveProtection_h_
