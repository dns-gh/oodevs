// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#include "clients_kernel_pch.h"
#include "MagicActionLocalMeteoType.h"

#include "OrderParameter.h"

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: MagicActionLocalMeteoType constructor
// Created: JSR 2010-04-09
// -----------------------------------------------------------------------------
MagicActionLocalMeteoType::MagicActionLocalMeteoType()
    : MagicActionMeteoType()
{
    name_ = "local_meteo";
    Initialize();
}

// -----------------------------------------------------------------------------
// Name: MagicActionMeteoType destructor
// Created: JSR 2010-04-09
// -----------------------------------------------------------------------------
MagicActionLocalMeteoType::~MagicActionLocalMeteoType()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MagicActionMeteoType::Initialize
// Created: JSR 2010-04-09
// -----------------------------------------------------------------------------
void MagicActionLocalMeteoType::Initialize()
{
    // Base parameters initialized in MagicActionMeteoType constructor
    Register( Count(), *new OrderParameter( "StartTime", "datetime", true ) );
    Register( Count(), *new OrderParameter( "EndTime", "datetime", true ) );
    Register( Count(), *new OrderParameter( "Location", "location", true ) );
}
