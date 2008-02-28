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
#include "MissionType.h"
#include "FragOrder.h"

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: Mission constructor
// Created: SBO 2006-11-29
// -----------------------------------------------------------------------------
Mission::Mission( const MissionType& type )
    : type_( type )
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
// Created: SBO 2007-04-24
// -----------------------------------------------------------------------------
unsigned int Mission::GetId() const
{
    return type_.GetId();
}

// -----------------------------------------------------------------------------
// Name: Mission::GetName
// Created: SBO 2007-04-24
// -----------------------------------------------------------------------------
std::string Mission::GetName() const
{
    return type_.GetName();
}

// -----------------------------------------------------------------------------
// Name: Mission::GetType
// Created: SBO 2007-04-23
// -----------------------------------------------------------------------------
const MissionType& Mission::GetType() const
{
    return type_;
}
