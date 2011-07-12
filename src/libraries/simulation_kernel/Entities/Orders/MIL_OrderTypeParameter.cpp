// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "MIL_OrderTypeParameter.h"
#include "MIL_OrderType_ABC.h"
#include "Entities/Orders/MIL_MissionParameter_ABC.h"
#include "Entities/Orders/MIL_ParameterType_ABC.h"
#include "Decision/DEC_Tools.h"
#include "protocol/Protocol.h"
#include <xeumeuleu/xml.hpp>

namespace
{
    bool InitIsList(  xml::xistream& xis )
    {
        std::string maxOccurs = xis.attribute< std::string >( "max-occurs", "1" );
        if( maxOccurs != "1" )
            return true;
        return false;
    }
}
// -----------------------------------------------------------------------------
// Name: MIL_OrderTypeParameter constructor
// Created: SBO 2008-03-03
// -----------------------------------------------------------------------------
MIL_OrderTypeParameter::MIL_OrderTypeParameter( xml::xistream& xis )
    : bIsOptional_  ( xis.attribute< bool >( "optional", false ) )
    , strName_      ( xis.attribute< std::string >( "name" ) )
    , strDiaName_   ( xis.attribute< std::string >( "dia-name" ) )
    , bIsList_      ( InitIsList( xis ) )
    , pParameter_   ( MIL_ParameterType_ABC::Find( xis.attribute< std::string >( "type" ) ) )
{
    if( !pParameter_ )
        xis.error( "Unknown parameter type" );
}

//-----------------------------------------------------------------------------
// Name: MIL_OrderTypeParameter constructor
// Created: NLD 2006-11-19
//-----------------------------------------------------------------------------
MIL_OrderTypeParameter::MIL_OrderTypeParameter( const MIL_OrderType_ABC& /*orderType*/, xml::xistream& xis )
    : bIsOptional_  ( xis.attribute< bool >( "optional", false ) )
    , strName_      ( xis.attribute< std::string >( "name" ) )
    , strDiaName_   ( xis.attribute< std::string >( "dia-name" ) )
    , bIsList_      ( InitIsList( xis ) )
    , pParameter_   ( MIL_ParameterType_ABC::Find( xis.attribute< std::string >( "type" ) ) )
{
    if( !pParameter_ )
        xis.error( "Unknown parameter type" );
    //$$$$ Checker type DIA si c possible
}

//-----------------------------------------------------------------------------
// Name: MIL_OrderTypeParameter destructor
// Created: NLD 2006-11-19
//-----------------------------------------------------------------------------
MIL_OrderTypeParameter::~MIL_OrderTypeParameter()
{
    // NOTHING
}

//-----------------------------------------------------------------------------
// Name: MIL_OrderTypeParameter::Copy
// Created: NLD 2006-11-19
//-----------------------------------------------------------------------------
bool MIL_OrderTypeParameter::Copy( const MIL_MissionParameter_ABC& from, sword::MissionParameter& to ) const
{
    if( !from.IsOfType( pParameter_->GetType() ) )
        return false;

    if( bIsList_ )
        to.set_null_value( !from.ToList( *to.mutable_value() ) );
    else
        to.set_null_value( !from.ToElement( *to.mutable_value()->Add() ) );
    if( to.null_value() )
        to.clear_value();
    return !to.null_value() || bIsOptional_;
}

// -----------------------------------------------------------------------------
// Name: MIL_OrderTypeParameter::GetName
// Created: LDC 2009-04-30
// -----------------------------------------------------------------------------
const std::string& MIL_OrderTypeParameter::GetName() const
{
    return strName_;
}

// -----------------------------------------------------------------------------
// Name: MIL_OrderTypeParameter::GetDIAName
// Created: LDC 2009-05-20
// -----------------------------------------------------------------------------
const std::string& MIL_OrderTypeParameter::GetDIAName() const
{
    return strDiaName_;
}

// -----------------------------------------------------------------------------
// Name: MIL_OrderTypeParameter::GetType
// Created: LDC 2009-04-30
// -----------------------------------------------------------------------------
const MIL_ParameterType_ABC& MIL_OrderTypeParameter::GetType() const
{
    return *pParameter_;
}

// -----------------------------------------------------------------------------
// Name: MIL_OrderTypeParameter::GetType
// Created: MGD 2010-11-12
// -----------------------------------------------------------------------------
bool MIL_OrderTypeParameter::IsList() const
{
    return bIsList_;
}
