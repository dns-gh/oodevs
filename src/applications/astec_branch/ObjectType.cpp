// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "astec_pch.h"
#include "ObjectType.h"


// -----------------------------------------------------------------------------
// Name: ObjectType constructor
// Created: AGE 2006-02-16
// -----------------------------------------------------------------------------
ObjectType::ObjectType( unsigned long id, const std::string& name, IDManager& manager )
    : id_( id )
    , name_( name )
    , manager_( manager )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ObjectType destructor
// Created: AGE 2006-02-16
// -----------------------------------------------------------------------------
ObjectType::~ObjectType()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ObjectType::GetName
// Created: AGE 2006-02-16
// -----------------------------------------------------------------------------
const std::string& ObjectType::GetName() const
{
    return name_;
}
