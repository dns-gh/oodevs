// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#include "clients_kernel_pch.h"
#include "MagicActionType.h"
#include "OrderParameter.h"

using namespace xml;
using namespace kernel;

// -----------------------------------------------------------------------------
// Name: MagicActionType constructor
// Created: JSR 2010-04-02
// -----------------------------------------------------------------------------
MagicActionType::MagicActionType()
    : OrderType( )
{
}

// -----------------------------------------------------------------------------
// Name: MagicActionType destructor
// Created: JSR 2010-04-02
// -----------------------------------------------------------------------------
MagicActionType::~MagicActionType()
{
    Clean();
}

// -----------------------------------------------------------------------------
// Name: MagicActionType::Initialize
// Created: JSR 2010-04-06
// -----------------------------------------------------------------------------
void MagicActionType::Initialize()
{
    OrderParameter* param = new OrderParameter( "Location", "Point", false );
    Register( Count(), *param );
}

// -----------------------------------------------------------------------------
// Name: MagicActionType::Clean
// Created: JSR 2010-04-06
// -----------------------------------------------------------------------------
void MagicActionType::Clean()
{
    DeleteAll();
}
