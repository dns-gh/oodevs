// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 MASA Group
//
// *****************************************************************************

#ifndef __MockAutomateFactory_h_
#define __MockAutomateFactory_h_

#include "simulation_kernel/AutomateFactory_ABC.h"

// =============================================================================
/** @class  MockAutomateFactory
    @brief  MockAutomateFactory
*/
// Created: MGD 2009-09-25
// =============================================================================
class MockAutomateFactory
    : public mockpp::ChainableMockObject
    , public AutomateFactory_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
    explicit MockAutomateFactory() 
        : mockpp::ChainableMockObject( MOCKPP_PCHAR( "MockAutomateFactory" ), 0 )
        , MOCKPP_CONSTRUCT_CHAINABLE_MEMBERS( CreateAutomateShadow )       
    {
    }
    virtual ~MockAutomateFactory() {}
    //@}
        
    virtual MIL_Automate& Create( xml::xistream& xis, MIL_Automate& parent )
    {
        return *CreateAutomateShadow();
    }

    virtual MIL_Automate& Create( xml::xistream& xis, MIL_Formation& parent )
    {
        return *CreateAutomateShadow();
    }

    MOCKPP_CONST_CHAINABLE_EXT0( MockAutomateFactory, MIL_Automate*, CreateAutomateShadow, MIL_Automate, );
};

#endif // __MockArmy_h_
