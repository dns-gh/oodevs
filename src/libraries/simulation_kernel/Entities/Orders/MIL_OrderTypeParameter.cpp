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

namespace
{
    unsigned int GetMaxOccurs( xml::xistream& xis )
    {
        if( !xis.has_attribute( "max-occurs" ) )
            return 1;
        if( xis.attribute< std::string >( "max-occurs" ) == "unbounded" )
            return std::numeric_limits< unsigned int >::max();
        return xis.attribute< unsigned int >( "max-occurs" );
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
    , minOccurs_    ( xis.attribute< unsigned int >( "min-occurs", 1 ) )
    , maxOccurs_    ( ::GetMaxOccurs( xis ) )
    , pParameter_   ( MIL_ParameterType_ABC::Find( xis.attribute< std::string >( "type" ) ) )
{
    if( !pParameter_ )
        throw MASA_EXCEPTION( xis.context() + "Unknown parameter type: " +
               xis.attribute< std::string >( "type" ) );
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

    if( IsList() )
        to.set_null_value( !from.ToList( *to.mutable_value() ) );
    else
        to.set_null_value( !from.ToElement( *to.add_value() ) );
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
    return maxOccurs_ > 1;
}

unsigned int MIL_OrderTypeParameter::GetMinOccurs() const
{
    return minOccurs_;
}

unsigned int  MIL_OrderTypeParameter::GetMaxOccurs() const
{
    return maxOccurs_;
}
