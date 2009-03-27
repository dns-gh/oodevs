// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 MASA Group
//
// *****************************************************************************

#ifndef __MockBuilder_h_
#define __MockBuilder_h_

#include "simulation_kernel/Entities/Objects/MIL_ObjectBuilder_ABC.h"

class Object;
class MIL_ObjectType_ABC;

// =============================================================================
/** @class  MockBuilder
    @brief  MockBuilder
*/
// Created: JCR 2008-09-01
// =============================================================================
class MockBuilder
    : public mockpp::ChainableMockObject
    , public MIL_ObjectBuilder_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
    explicit MockBuilder() 
        : mockpp::ChainableMockObject( MOCKPP_PCHAR( "MockBuilder" ) )
        , GetType_mocker( "GetType", this )
        , MOCKPP_CONSTRUCT_CHAINABLE_MEMBERS( Build )
    {
    }
    virtual ~MockBuilder() {}
    //@}

    virtual const MIL_ObjectType_ABC& GetType() const
    {
        return *GetType_mocker.forward();
    }

    MOCKPP_VOID_CONST_CHAINABLE_EXT1    ( MockBuilder, Build, Object&, ,Object );

    mockpp::ChainableMockMethod< const MIL_ObjectType_ABC* > GetType_mocker;
};

#endif // __MockBuilder_h_
