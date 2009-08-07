// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#ifndef __StubMIL_ObjectType_ABC_h_
#define __StubMIL_ObjectType_ABC_h_

#include "Entities/Objects/MIL_ObjectType_ABC.h"

// =============================================================================
/** @class  MockMIL_ObjectType_ABC
    @brief  MockMIL_ObjectType_ABC
*/
// Created: LDC 2009-06-11
// =============================================================================
class MockMIL_ObjectType_ABC : public MIL_ObjectType_ABC
    , public mockpp::ChainableMockObject
{

public:
    //! @name Constructors/Destructor
    //@{
             MockMIL_ObjectType_ABC() 
                : mockpp::ChainableMockObject( "MockMIL_ObjectType_ABC", 0 )
                , GetName_mocker( "GetName", this )
             {}
    virtual ~MockMIL_ObjectType_ABC() {}
    //@}
    virtual uint GetID() const
    {
        throw;
    }
    virtual const std::string& GetName() const
    {
        return *GetName_mocker.forward();
    }

    mockpp::ChainableMockMethod< const std::string* > GetName_mocker;
};

#endif // __StubMIL_ObjectType_ABC_h_
