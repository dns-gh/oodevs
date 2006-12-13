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
#include "xeumeuleu/xml.h"

using namespace kernel;
using namespace xml;

// -----------------------------------------------------------------------------
// Name: FragOrder constructor
// Created: SBO 2006-11-29
// -----------------------------------------------------------------------------
FragOrder::FragOrder( xml::xistream& xis )
    : isDefaultOrder_  ( false )
    , isWithoutMission_( false )
{
    std::string name;
    xis >> attribute( "name", name )
        >> attribute( "id", (int&)id_ )
        >> optional() >> attribute( "available-for-all-mission", isDefaultOrder_ )
        >> optional() >> attribute( "available-without-mission", isWithoutMission_ );
    name_ = name.c_str();
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
// Created: AGE 2006-04-05
// -----------------------------------------------------------------------------
unsigned long FragOrder::GetId() const
{
    return id_;
}

// -----------------------------------------------------------------------------
// Name: FragOrder::GetName
// Created: AGE 2006-04-05
// -----------------------------------------------------------------------------
QString FragOrder::GetName() const
{
    return name_;
}

// -----------------------------------------------------------------------------
// Name: FragOrder::IsDefaultOrder
// Created: SBO 2006-12-01
// -----------------------------------------------------------------------------
bool FragOrder::IsDefaultOrder() const
{
    return isDefaultOrder_;
}
    
// -----------------------------------------------------------------------------
// Name: FragOrder::IsMissionRequired
// Created: SBO 2006-12-01
// -----------------------------------------------------------------------------
bool FragOrder::IsMissionRequired() const
{
    return !isWithoutMission_;
}
