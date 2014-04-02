// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "clients_kernel_pch.h"
#include "OrderType.h"
#include "OrderParameter.h"
#include <xeumeuleu/xml.hpp>

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: OrderType constructor
// Created: SBO 2007-04-23
// -----------------------------------------------------------------------------
OrderType::OrderType( xml::xistream& xis )
{
    xis >> xml::attribute( "name", name_ )
        >> xml::attribute( "id", id_ )
        >> xml::optional >> xml::start( "description" )
            >> xml::optional >> xml::attribute( "doctrine", doctrine_ )
            >> xml::optional >> xml::attribute( "usage", usage_ )
        >> xml ::end;
}

// -----------------------------------------------------------------------------
// Name: OrderType constructor
// Created: JSR 2010-04-06
// -----------------------------------------------------------------------------
OrderType::OrderType()
    : id_( 0 )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: OrderType constructor
// Created: JSR 2011-03-16
// -----------------------------------------------------------------------------
OrderType::OrderType( const std::string& name, unsigned long id )
    : name_( name )
    , id_  ( id )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: OrderType destructor
// Created: SBO 2007-04-23
// -----------------------------------------------------------------------------
OrderType::~OrderType()
{
    DeleteAll();
}

// -----------------------------------------------------------------------------
// Name: OrderType::GetId
// Created: SBO 2007-04-24
// -----------------------------------------------------------------------------
unsigned long OrderType::GetId() const
{
    return id_;
}

// -----------------------------------------------------------------------------
// Name: OrderType::GetName
// Created: SBO 2007-04-24
// -----------------------------------------------------------------------------
const std::string& OrderType::GetName() const
{
    return name_;
}

// -----------------------------------------------------------------------------
// Name: OrderType::GetDoctrineInformation
// Created: NPT 2013-02-01
// -----------------------------------------------------------------------------
const std::string& OrderType::GetDoctrineInformation() const
{
    return doctrine_;
}

// -----------------------------------------------------------------------------
// Name: OrderType::GetUsageInformation
// Created: NPT 2013-02-01
// -----------------------------------------------------------------------------
const std::string& OrderType::GetUsageInformation() const
{
    return usage_;
}

// -----------------------------------------------------------------------------
// Name: OrderType::GetType
// Created: ABR 2014-01-14
// -----------------------------------------------------------------------------
E_MissionType OrderType::GetType() const
{
    return eNbrMissionType;
}
