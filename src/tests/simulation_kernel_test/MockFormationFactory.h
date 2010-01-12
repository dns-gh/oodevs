// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 MASA Group
//
// *****************************************************************************

#ifndef __MockFormationFactory_h_
#define __MockFormationFactory_h_

#include "simulation_kernel/FormationFactory_ABC.h"

// =============================================================================
/** @class  MockFormationFactory
    @brief  MockFormationFactory
*/
// Created: MGD 2009-09-25
// =============================================================================
class MockFormationFactory
    : public mockpp::ChainableMockObject
    , public FormationFactory_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
    explicit MockFormationFactory() 
        : mockpp::ChainableMockObject( MOCKPP_PCHAR( "MockFormationFactory" ), 0 )
        , MOCKPP_CONSTRUCT_CHAINABLE_MEMBERS( CreateFormationShadow )       
    {
    }
    virtual ~MockFormationFactory() {}
    //@}
        
    virtual MIL_Formation& Create( xml::xistream& xis, MIL_Army_ABC& army, MIL_Formation* parent = 0  )
    {
        return *CreateFormationShadow();
    }

    MOCKPP_CONST_CHAINABLE_EXT0( MockFormationFactory, MIL_Formation*, CreateFormationShadow, MIL_Formation, );
};

#endif // __MockArmy_h_
