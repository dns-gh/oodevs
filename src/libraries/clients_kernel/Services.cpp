// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "clients_kernel_pch.h"
#include "Services.h"

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: Services constructor
// Created: AGE 2008-08-13
// -----------------------------------------------------------------------------
Services::Services()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Services destructor
// Created: AGE 2008-08-13
// -----------------------------------------------------------------------------
Services::~Services()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Services::Has
// Created: AGE 2008-08-13
// -----------------------------------------------------------------------------
bool Services::Has( sword::Service id ) const
{
    return services_.find( id ) != services_.end();
}

// -----------------------------------------------------------------------------
// Name: Services::HasService
// Created: AGE 2008-08-13
// -----------------------------------------------------------------------------
void Services::Declare( sword::Service id )
{
    services_.insert( id );
}

// -----------------------------------------------------------------------------
// Name: Services::Clear
// Created: BAX 2013-10-25
// -----------------------------------------------------------------------------
void Services::Clear()
{
    services_.clear();
}