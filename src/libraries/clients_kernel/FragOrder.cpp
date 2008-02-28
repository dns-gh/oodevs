// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "clients_kernel_pch.h"
#include "FragOrder.h"
#include "FragOrderType.h"

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: FragOrder constructor
// Created: SBO 2006-11-29
// -----------------------------------------------------------------------------
FragOrder::FragOrder( const FragOrderType& type )
    : type_( type )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: FragOrder destructor
// Created: AGE 2006-03-14
// -----------------------------------------------------------------------------
FragOrder::~FragOrder()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: FragOrder::GetId
// Created: SBO 2007-04-24
// -----------------------------------------------------------------------------
unsigned int FragOrder::GetId() const
{
    return type_.GetId();
}

// -----------------------------------------------------------------------------
// Name: FragOrder::GetName
// Created: SBO 2007-04-24
// -----------------------------------------------------------------------------
std::string FragOrder::GetName() const
{
    return type_.GetName();
}

// -----------------------------------------------------------------------------
// Name: FragOrder::GetType
// Created: SBO 2007-04-23
// -----------------------------------------------------------------------------
const FragOrderType& FragOrder::GetType() const
{
    return type_;
}
