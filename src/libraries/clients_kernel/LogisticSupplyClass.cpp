// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#include "clients_kernel_pch.h"
#include "LogisticSupplyClass.h"
#include <xeumeuleu/xml.hpp>

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: LogisticSupplyClass constructor
// Created: JSR 2010-06-07
// -----------------------------------------------------------------------------
LogisticSupplyClass::LogisticSupplyClass( xml::xistream& input )
{
    input >> xml::attribute( "type", name_ )
          >> xml::attribute( "id", id_ );
}

// -----------------------------------------------------------------------------
// Name: LogisticSupplyClass destructor
// Created: JSR 2010-06-07
// -----------------------------------------------------------------------------
LogisticSupplyClass::~LogisticSupplyClass()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: LogisticSupplyClass::GetName
// Created: JSR 2010-06-07
// -----------------------------------------------------------------------------
const std::string& LogisticSupplyClass::GetName() const
{
    return name_;
}

// -----------------------------------------------------------------------------
// Name: LogisticSupplyClass::GetId
// Created: JSR 2010-06-07
// -----------------------------------------------------------------------------
unsigned int LogisticSupplyClass::GetId() const
{
    return id_;
}
