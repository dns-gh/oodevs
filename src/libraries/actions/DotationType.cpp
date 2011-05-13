// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#include "actions_pch.h"
#include "DotationType.h"
#include "clients_kernel/DotationType.h"
#include "ParameterVisitor_ABC.h"
#include "protocol/Protocol.h"
#pragma warning( push )
#pragma warning( disable : 4702 )
#include <boost/lexical_cast.hpp>
#pragma warning( pop )
#include <xeumeuleu/xml.hpp>

using namespace actions;
using namespace parameters;

// -----------------------------------------------------------------------------
// Name: DotationType constructor
// Created: SBO 2007-05-21
// -----------------------------------------------------------------------------
DotationType::DotationType( const kernel::OrderParameter& parameter, unsigned int id, const tools::Resolver_ABC< kernel::DotationType >& resolver )
    : Parameter< std::string >( parameter )
    , type_( resolver.Get( id ) )
{
    SetValue( type_.GetName() );
}

// -----------------------------------------------------------------------------
// Name: DotationType constructor
// Created: SBO 2007-05-21
// -----------------------------------------------------------------------------
DotationType::DotationType( const kernel::OrderParameter& parameter, xml::xistream& xis, const tools::Resolver_ABC< kernel::DotationType >& resolver )
    : Parameter< std::string >( parameter )
    , type_( resolver.Get( xis.attribute< unsigned int >( "value" ) ) )
{
    SetValue( type_.GetName() );
}

// -----------------------------------------------------------------------------
// Name: DotationType destructor
// Created: SBO 2007-05-21
// -----------------------------------------------------------------------------
DotationType::~DotationType()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: DotationType::Serialize
// Created: SBO 2007-05-21
// -----------------------------------------------------------------------------
void DotationType::Serialize( xml::xostream& xos ) const
{
    Parameter< std::string >::Serialize( xos );
    xos << xml::attribute( "value", type_.GetId() );
}

// -----------------------------------------------------------------------------
// Name: DotationType::CommitTo
// Created: AGE 2007-10-23
// -----------------------------------------------------------------------------
void DotationType::CommitTo( std::string& content ) const
{
    content += boost::lexical_cast< std::string >( type_.GetId() );
}

// -----------------------------------------------------------------------------
// Name: DotationType::CommitTo
// Created: SBO 2007-05-22
// -----------------------------------------------------------------------------
void DotationType::CommitTo( sword::MissionParameter& message ) const
{
    message.set_null_value( !IsSet() );
   message.mutable_value()->Add()->mutable_resourcetype()->set_id( type_.GetId() );
}
// -----------------------------------------------------------------------------
// Name: DotationType::CommitTo
// Created: SBO 2007-05-22
// -----------------------------------------------------------------------------
void DotationType::CommitTo( sword::MissionParameter_Value& message ) const
{
    message.mutable_resourcetype()->set_id( type_.GetId() );
}

// -----------------------------------------------------------------------------
// Name: DotationType::Accept
// Created: AGE 2007-10-23
// -----------------------------------------------------------------------------
void DotationType::Accept( ParameterVisitor_ABC& visitor ) const
{
    visitor.Visit( *this );
}

// -----------------------------------------------------------------------------
// Name: DotationType::SerializeType
// Created: LDC 2011-05-13
// -----------------------------------------------------------------------------
std::string DotationType::SerializeType() const
{
    return "resourcetype";
}
