// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 MASA Group
//
// *****************************************************************************

#ifndef __MockPopulationFactory_h_
#define __MockPopulationFactory_h_

#include "simulation_kernel/PopulationFactory_ABC.h"

// =============================================================================
/** @class  MockPopulationFactory
    @brief  MockPopulationFactory
*/
// Created: MGD 2009-09-25
// =============================================================================
class MockPopulationFactory
    : public mockpp::ChainableMockObject
    , public PopulationFactory_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
    explicit MockPopulationFactory() 
        : mockpp::ChainableMockObject( MOCKPP_PCHAR( "MockPopulationFactory" ), 0 )
        , MOCKPP_CONSTRUCT_CHAINABLE_MEMBERS( CreatePopulationShadow )       
    {
    }
    virtual ~MockPopulationFactory() {}
    //@}
        
    virtual MIL_Population& Create( xml::xistream& xis, MIL_Army& army )
    {
        return *CreatePopulationShadow();
    }

    MOCKPP_CONST_CHAINABLE_EXT0( MockPopulationFactory, MIL_Population*, CreatePopulationShadow, MIL_Population, );
};

#endif // __MockArmy_h_
