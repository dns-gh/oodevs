// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#include "clients_kernel_pch.h"
#include "InfrastructureType.h"
#include <xeumeuleu/xml.hpp>

using namespace kernel;

unsigned int InfrastructureType::nNextID_ = 0;

// -----------------------------------------------------------------------------
// Name: InfrastructureType constructor
// Created: SLG 2010-12-29
// -----------------------------------------------------------------------------
InfrastructureType::InfrastructureType( xml::xistream& xis )
    : id_    ( nNextID_++ )
    , name_  ( xis.attribute< std::string >( "name" ) )
    , symbol_( "infrastructures/" + xis.attribute< std::string >( "symbol" ) )
{
    xis >> xml::optional >> xml::start( "capacities" )
        >> xml::list( *this, &InfrastructureType::ReadCapacity )
        >> xml::end;
}

// -----------------------------------------------------------------------------
// Name: InfrastructureType destructor
// Created: SLG 2010-12-29
// -----------------------------------------------------------------------------
InfrastructureType::~InfrastructureType()
{
    //NOTHING
}

// -----------------------------------------------------------------------------
// Name: InfrastructureType::ReadCapacity
// Created: SLG 2010-12-29
// -----------------------------------------------------------------------------
void InfrastructureType::ReadCapacity( const std::string& capacity, xml::xistream& xis )
{
    capacities_[ capacity ].reset( new xml::xibufferstream( xis ) );
}

// -----------------------------------------------------------------------------
// Name: InfrastructureType::FindCapacity
// Created: SLG 2010-12-29
// -----------------------------------------------------------------------------
xml::xistream* InfrastructureType::FindCapacity( const std::string& capacity ) const
{
    CIT_Capacities it = capacities_.find( capacity );
    if( it != capacities_.end() )
        return it->second.get();
    return 0;
}

// -----------------------------------------------------------------------------
// Name: InfrastructureType::GetName
// Created: SLG 2010-12-29
// -----------------------------------------------------------------------------
const std::string& InfrastructureType::GetName() const
{
    return name_;
}

// -----------------------------------------------------------------------------
// Name: InfrastructureType::GetId
// Created: SLG 2010-12-29
// -----------------------------------------------------------------------------
unsigned int InfrastructureType::GetId() const
{
    return id_;
}

// -----------------------------------------------------------------------------
// Name: InfrastructureType::GetSymbol
// Created: SLG 2011-01-11
// -----------------------------------------------------------------------------
const std::string& InfrastructureType::GetSymbol() const
{
    return symbol_;
}
