// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "clients_kernel_pch.h"
#include "OrderParameter.h"
#include "OrderParameterValue.h"
#pragma warning( push, 1 )
#pragma warning( disable : 4512 )
#include <boost/algorithm/string.hpp>
#pragma warning( pop )
#include <xeumeuleu/xml.hpp>

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: OrderParameter constructor
// Created: SBO 2007-04-23
// -----------------------------------------------------------------------------
OrderParameter::OrderParameter( xml::xistream& xis )
    : name_( xis.attribute< std::string >( "name" ) )
    , type_( boost::algorithm::to_lower_copy( xis.attribute< std::string >( "type" ) ) )
    , optional_( xis.attribute( "optional", false ) )
    , minOccurs_( 1 )
    , maxOccurs_( 1 )
{
    xis >> xml::list( "value", *this, &OrderParameter::ReadValue )
        >> xml::optional >> xml::start( "choice" )
            >> xml::list( "parameter", *this, &OrderParameter::ReadChoice )
        >> xml::end()
        >> xml::optional >> xml::attribute< unsigned int >( "min-occurs", minOccurs_ );
    std::string maxString( "1" );
    xis >> xml::optional >> xml::attribute< std::string >( "max-occurs", maxString );
    if( maxString == "unbounded" )
        maxOccurs_ = std::numeric_limits< unsigned int >::max();
    else
        xis >> xml::optional >> xml::attribute< unsigned int >( "max-occurs", maxOccurs_ );
}

// -----------------------------------------------------------------------------
// Name: OrderParameter constructor
// Created: SBO 2007-04-24
// -----------------------------------------------------------------------------
OrderParameter::OrderParameter( const std::string& name, const std::string& type, bool optional, unsigned int min /*= 1*/, unsigned int max /*= 1*/ )
    : name_    ( name )
    , type_    ( boost::algorithm::to_lower_copy( type ) )
    , optional_( optional )
    , minOccurs_( min )
    , maxOccurs_( max )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: OrderParameter destructor
// Created: SBO 2007-04-23
// -----------------------------------------------------------------------------
OrderParameter::~OrderParameter()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: OrderParameter::GetName
// Created: SBO 2007-04-23
// -----------------------------------------------------------------------------
std::string OrderParameter::GetName() const
{
    return name_;
}

// -----------------------------------------------------------------------------
// Name: OrderParameter::GetType
// Created: SBO 2007-04-23
// -----------------------------------------------------------------------------
std::string OrderParameter::GetType() const
{
    return type_;
}

// -----------------------------------------------------------------------------
// Name: OrderParameter::IsOptional
// Created: SBO 2007-04-23
// -----------------------------------------------------------------------------
bool OrderParameter::IsOptional() const
{
    return optional_;
}

// -----------------------------------------------------------------------------
// Name: OrderParameter::MinOccurs
// Created: LDC 2010-09-17
// -----------------------------------------------------------------------------
unsigned int OrderParameter::MinOccurs() const
{
    return minOccurs_;
}

// -----------------------------------------------------------------------------
// Name: OrderParameter::MaxOccurs
// Created: LDC 2010-09-17
// -----------------------------------------------------------------------------
unsigned int OrderParameter::MaxOccurs() const
{
    return maxOccurs_;
}

// -----------------------------------------------------------------------------
// Name: OrderParameter::GetValue
// Created: SBO 2007-05-25
// -----------------------------------------------------------------------------
const OrderParameterValue& OrderParameter::GetValue( unsigned int id ) const
{
    CIT_OrderParameterValues it = values_.find( id );
    if( it == values_.end() )
        throw std::runtime_error( "Undefined enumeration value." ); // $$$$ SBO 2007-05-25:
    return it->second;
}

// -----------------------------------------------------------------------------
// Name: OrderParameter::ReadValue
// Created: SBO 2007-04-23
// -----------------------------------------------------------------------------
void OrderParameter::ReadValue( xml::xistream& xis )
{
    OrderParameterValue value( xis );
    values_.insert( std::make_pair( value.GetId(), value ) );
}

// -----------------------------------------------------------------------------
// Name: OrderParameter::ReadChoice
// Created: LDC 2010-08-18
// -----------------------------------------------------------------------------
void OrderParameter::ReadChoice( xml::xistream& xis )
{
    std::string type;
    xis >> xml::attribute( "type", type );
    aliases_.insert( boost::algorithm::to_lower_copy( type ) );
}

// -----------------------------------------------------------------------------
// Name: OrderParameter::AddValue
// Created: JSR 2010-04-09
// -----------------------------------------------------------------------------
void OrderParameter::AddValue( int id, const std::string& name )
{
    OrderParameterValue value( id, name );
    values_.insert( std::make_pair( value.GetId(), value ) );
}

// -----------------------------------------------------------------------------
// Name: OrderParameter::Accept
// Created: SBO 2007-05-25
// -----------------------------------------------------------------------------
void OrderParameter::Accept( OrderParameterValueVisitor_ABC& visitor ) const
{
    for( CIT_OrderParameterValues it = values_.begin(); it != values_.end(); ++it )
        visitor.Visit( it->second );
}

// -----------------------------------------------------------------------------
// Name: OrderParameter::Accept
// Created: LDC 2010-08-18
// -----------------------------------------------------------------------------
void OrderParameter::Accept( ChoicesVisitor_ABC& visitor ) const
{
    for( CIT_Aliases it = aliases_.begin(); it != aliases_.end(); ++it )
        visitor.Visit( *it );
}

// -----------------------------------------------------------------------------
// Name: OrderParameter::AddChoice
// Created: LDC 2010-09-28
// -----------------------------------------------------------------------------
void OrderParameter::AddChoice( const std::string& choice )
{
    aliases_.insert( choice );
}

// -----------------------------------------------------------------------------
// Name: OrderParameter::IsList
// Created: LDC 2010-09-14
// -----------------------------------------------------------------------------
bool OrderParameter::IsList() const
{
    return ( maxOccurs_ != 1 || minOccurs_ != 1 );
}

// -----------------------------------------------------------------------------
// Name: OrderParameter::IsCompatible
// Created: PHC 2010-10-18
// -----------------------------------------------------------------------------
std::string OrderParameter::CompatibleType( const std::string& type ) const
{
    if( type == type_ )
        return type;
    if( std::find( aliases_.begin(), aliases_.end(), type ) != aliases_.end() )
        return type;
    if( type_ == "phaseline" )
        return "phaseline";
    if( type == "phaselinelist" )
        return "phaseline";
    if( type == "direction" )
        return "heading";
    if( type == "intelligencelist" )
        return "intelligence";
    if( type == "location" && type_ == "polygon" )
        return "polygon";
    if( type == "list" )
        return type_;
    return "";
}
