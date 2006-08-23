// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "clients_kernel_pch.h"
#include "Mission.h"
#include "FragOrder.h"

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: Mission constructor
// Created: AGE 2006-03-14
// -----------------------------------------------------------------------------
Mission::Mission( const QString& name, unsigned int id, bool automat )
    : name_( name )
    , id_( id )
    , automat_( automat )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Mission destructor
// Created: AGE 2006-03-14
// -----------------------------------------------------------------------------
Mission::~Mission()
{
    DeleteAll();
}


// -----------------------------------------------------------------------------
// Name: Mission::GetId
// Created: AGE 2006-04-05
// -----------------------------------------------------------------------------
unsigned long Mission::GetId() const
{
    return id_;
}
 
// -----------------------------------------------------------------------------
// Name: Mission::GetName
// Created: AGE 2006-04-05
// -----------------------------------------------------------------------------
QString Mission::GetName() const
{
    return name_;
}

// -----------------------------------------------------------------------------
// Name: Mission::IsAutomat
// Created: AGE 2006-04-05
// -----------------------------------------------------------------------------
bool Mission::IsAutomat() const
{
    return automat_;
}
