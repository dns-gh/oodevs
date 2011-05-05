// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifdef __GNUG__
#   pragma implementation
#endif

#include "edxlhave_app_pch.h"
#include "Hospital.h"

using namespace edxl;


// -----------------------------------------------------------------------------
// Name: Hospital constructor
// Created: BCI 2011-05-02
// -----------------------------------------------------------------------------
Hospital::Hospital( unsigned int id, const std::string& name )
: id_( id )
, name_( name )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Hospital destructor
// Created: BCI 2011-05-02
// -----------------------------------------------------------------------------
Hospital::~Hospital()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Hospital::GetId
// Created: BCI 2011-05-02
// -----------------------------------------------------------------------------
unsigned int Hospital::GetId() const
{
    return id_;
}

// -----------------------------------------------------------------------------
// Name: Hospital::GetName
// Created: BCI 2011-05-02
// -----------------------------------------------------------------------------
const std::string& Hospital::GetName() const
{
    return name_;
}
