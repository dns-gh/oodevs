// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#include "wise_driver_dll_pch.h"
#include "TaskFactory.h"

// -----------------------------------------------------------------------------
// Name: TaskFactory constructor
// Created: SEB 2010-10-15
// -----------------------------------------------------------------------------
TaskFactory::TaskFactory( const Model& model, SwordMessagePublisher_ABC& publisher )
    : model_( model )
    , publisher_( publisher )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: TaskFactory destructor
// Created: SEB 2010-10-15
// -----------------------------------------------------------------------------
TaskFactory::~TaskFactory()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: TaskFactory::OnCreate
// Created: SEB 2010-10-15
// -----------------------------------------------------------------------------
void TaskFactory::OnCreate( CWISEDriver& /*driver*/, const WISE_HANDLE& /*database*/, const WISE_HANDLE& /*handle*/ )
{
    // $$$$ SEB 2010-10-27: TODO: handle task creation
}
