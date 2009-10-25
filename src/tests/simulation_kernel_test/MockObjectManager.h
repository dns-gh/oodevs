// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 MASA Group
//
// *****************************************************************************

#ifndef __MockObjectManager_h_
#define __MockObjectManager_h_

#include "simulation_kernel/Entities/Objects/MIL_ObjectManager.h"

// =============================================================================
/** @class  MockObjectManager
    @brief  MockObjectManager
*/
// Created: MGD 2009-09-25
// =============================================================================
class MockObjectManager
    : public mockpp::ChainableMockObject
    , public MIL_ObjectManager
{
public:
    //! @name Constructors/Destructor
    //@{
    explicit MockObjectManager() 
        : mockpp::ChainableMockObject( MOCKPP_PCHAR( "MockObjectManager" ), 0 )
        , MOCKPP_CONSTRUCT_CHAINABLE_MEMBERS( CreateObjectShadow )       
    {
    }
    virtual ~MockObjectManager() {}
    //@}
        
    MIL_Object_ABC& CreateObject( xml::xistream& xis, MIL_Army_ABC& army )
    {
        return *CreateObjectShadow();
    }

    MOCKPP_CONST_CHAINABLE_EXT0( MockObjectManager, MIL_Object_ABC*, CreateObjectShadow, MIL_Object_ABC, );
};

#endif // __MockArmy_h_
