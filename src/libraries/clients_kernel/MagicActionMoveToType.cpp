// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#include "clients_kernel_pch.h"
#include "MagicActionMoveToType.h"
#include "OrderParameter.h"

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: MagicActionMoveToType constructor
// Created: JSR 2010-04-08
// -----------------------------------------------------------------------------
MagicActionMoveToType::MagicActionMoveToType()
    : MagicActionType()
{
    name_ = "teleport";
    Initialize();
}

// -----------------------------------------------------------------------------
// Name: MagicActionMoveToType destructor
// Created: JSR 2010-04-08
// -----------------------------------------------------------------------------
MagicActionMoveToType::~MagicActionMoveToType()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MagicActionMoveToType::Initialize
// Created: JSR 2010-04-08
// -----------------------------------------------------------------------------
void MagicActionMoveToType::Initialize()
{
    OrderParameter* param = new OrderParameter( "Location", "Point", false );
    Register( Count(), *param );
}
