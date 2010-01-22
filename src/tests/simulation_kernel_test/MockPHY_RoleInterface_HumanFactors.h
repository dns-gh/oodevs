// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#ifndef __MockPHY_RoleInterface_HumanFactors_h_
#define __MockPHY_RoleInterface_HumanFactors_h_

#include "Entities/Agents/Roles/HumanFactors/PHY_RoleInterface_HumanFactors.h"

// =============================================================================
/** @class  MockPHY_RoleInterface_HumanFactors
@brief  MockPHY_RoleInterface_HumanFactors
*/
// Created: SLG 2010-01-20
// =============================================================================
class MockPHY_RoleInterface_HumanFactors
    : public mockpp::ChainableMockObject
    , public PHY_RoleInterface_HumanFactors
{

public:
    //! @name Constructors/Destructor
    //@{
    MockPHY_RoleInterface_HumanFactors()
        : mockpp::ChainableMockObject( MOCKPP_PCHAR( "MockPHY_RoleInterface_HumanFactors" ), 0 )
        , MOCKPP_CONSTRUCT_CHAINABLE_MEMBERS( Update )
        , MOCKPP_CONSTRUCT_CHAINABLE_MEMBERS( Clean )
        , MOCKPP_CONSTRUCT_CHAINABLE_MEMBERS( HasChanged )
        , MOCKPP_CONSTRUCT_CHAINABLE_MEMBERS( ReadOverloading )
        , MOCKPP_CONSTRUCT_CHAINABLE_MEMBERS( SetMorale )
        , MOCKPP_CONSTRUCT_CHAINABLE_MEMBERS( SetExperience )
        , MOCKPP_CONSTRUCT_CHAINABLE_MEMBERS( SetTiredness )
        , MOCKPP_CONSTRUCT_CHAINABLE_MEMBERS( GetMoraleShadow )
        , MOCKPP_CONSTRUCT_CHAINABLE_MEMBERS( GetTirednessShadow )
        , MOCKPP_CONSTRUCT_CHAINABLE_MEMBERS( ModifyPH )

    {}
    virtual ~MockPHY_RoleInterface_HumanFactors() {}
    //@}


    virtual const PHY_Morale& GetMorale() const
    {
        PHY_Morale* result = GetMoraleShadow();
        return *result;
    }

    virtual const PHY_Tiredness& GetTiredness() const
    {
        PHY_Tiredness* result = GetTirednessShadow();
        return *result;
    }


    //! @name Operations
    //@{
    MOCKPP_VOID_CHAINABLE_EXT1  ( MockPHY_RoleInterface_HumanFactors    , Update, bool, , bool );
    MOCKPP_VOID_CHAINABLE0      ( MockPHY_RoleInterface_HumanFactors    , Clean );
    MOCKPP_CONST_CHAINABLE0     ( MockPHY_RoleInterface_HumanFactors    , bool, HasChanged );
    MOCKPP_VOID_CHAINABLE_EXT1  ( MockPHY_RoleInterface_HumanFactors    , ReadOverloading, xml::xistream&, , xml::xistream );
    MOCKPP_VOID_CHAINABLE1      ( MockPHY_RoleInterface_HumanFactors    , SetMorale, PHY_Morale );
    MOCKPP_VOID_CHAINABLE1      ( MockPHY_RoleInterface_HumanFactors    , SetExperience, PHY_Experience );
    MOCKPP_VOID_CHAINABLE1      ( MockPHY_RoleInterface_HumanFactors    , SetTiredness, PHY_Tiredness );
    MOCKPP_CONST_CHAINABLE0     ( MockPHY_RoleInterface_HumanFactors    , PHY_Morale*, GetMoraleShadow );
    MOCKPP_CONST_CHAINABLE0     ( MockPHY_RoleInterface_HumanFactors    , PHY_Tiredness*, GetTirednessShadow );
    MOCKPP_CONST_CHAINABLE_EXT1 ( MockPHY_RoleInterface_HumanFactors    , MT_Float, ModifyPH, MT_Float, MT_Float, ,MT_Float );
    //@}

};


#endif // __MockPHY_RoleInterface_HumanFactors_h_
