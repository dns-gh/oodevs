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
    , medical_( xis.attribute< bool >( "medical", false ) )
{
    // NOTHING
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

// -----------------------------------------------------------------------------
// Name: InfrastructureType::IsMedical
// Created: ABR 2012-11-22
// -----------------------------------------------------------------------------
bool InfrastructureType::IsMedical() const
{
    return medical_;
}
