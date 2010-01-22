// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#ifndef __MockPHY_Composante_ABC_h_
#define __MockPHY_Composante_ABC_h_

#include"Entities/Agents/Units/Composantes/PHY_Composante_ABC.h"

// =============================================================================
/** @class  MockPHY_Composante_ABC
    @brief  MockPHY_Composante_ABC
*/
// Created: SLG 2010-01-19
// =============================================================================
class MockPHY_Composante_ABC
    : public mockpp::ChainableMockObject
    , public PHY_Composante_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             MockPHY_Composante_ABC()
                 : mockpp::ChainableMockObject( MOCKPP_PCHAR( "MockPHY_Composante_ABC" ) )
             , MOCKPP_CONSTRUCT_CHAINABLE_MEMBERS( GetTypeShadow )
                 , MOCKPP_CONSTRUCT_CHAINABLE_MEMBERS( CanFire )
                 , MOCKPP_CONSTRUCT_CHAINABLE_MEMBERS( CanFireWhenUnloaded )
                 , MOCKPP_CONSTRUCT_CHAINABLE_MEMBERS( GetMajorScore )
             {}
             virtual ~MockPHY_Composante_ABC() {}
    //@}

    //! @name Operations
    //@{
    virtual const PHY_ComposanteType_ABC& GetType            () const
    {
        PHY_ComposanteType_ABC* result = GetTypeShadow();
        return *result;
    }
    MOCKPP_CONST_CHAINABLE0( MockPHY_Composante_ABC, PHY_ComposanteType_ABC*, GetTypeShadow );
    MOCKPP_CONST_CHAINABLE0( MockPHY_Composante_ABC, bool, CanFire );
    MOCKPP_CONST_CHAINABLE0( MockPHY_Composante_ABC, bool, CanFireWhenUnloaded );
    MOCKPP_CONST_CHAINABLE0( MockPHY_Composante_ABC, unsigned int, GetMajorScore );
    //@}
};

#endif // __MockPHY_Composante_ABC_h_
