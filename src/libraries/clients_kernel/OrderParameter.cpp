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
#include <boost/bind.hpp>
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
    , minValue_ ( std::numeric_limits< double >::min() )
    , maxValue_ ( std::numeric_limits< double >::max() )
    , indirectFire_( false )
{
    xis >> xml::list( "value", *this, &OrderParameter::ReadValue )
        >> xml::optional >> xml::start( "choice" )
            >> xml::list( "parameter", boost::bind( &OrderParameter::ReadChoice, this, _1, boost::ref( aliases_ ), true ) )
        >> xml::end
        >> xml::optional >> xml::start( "objects" )
                >> xml::list( "parameter", boost::bind( &OrderParameter::ReadChoice, this, _1, boost::ref( genObjects_ ), false ) )
            >> xml::end
        >> xml::optional >> xml::attribute( "min-occurs", minOccurs_ )
        >> xml::optional >> xml::attribute( "min-value", minValue_ )
        >> xml::optional >> xml::attribute( "max-value", maxValue_ )
        >> xml::optional >> xml::attribute( "indirect-fire-only", indirectFire_ );
    std::string maxString( "1" );
    xis >> xml::optional >> xml::attribute( "max-occurs", maxString );
    if( maxString == "unbounded" )
        maxOccurs_ = std::numeric_limits< unsigned int >::max();
    else
        xis >> xml::optional >> xml::attribute( "max-occurs", maxOccurs_ );
}

// -----------------------------------------------------------------------------
// Name: OrderParameter constructor
// Created: SBO 2007-04-24
// -----------------------------------------------------------------------------
OrderParameter::OrderParameter( const std::string& name, const std::string& type, bool optional, unsigned int min /* = 1*/, unsigned int max /* = 1*/ )
    : name_    ( name )
    , type_    ( boost::algorithm::to_lower_copy( type ) )
    , optional_( optional )
    , minOccurs_( min )
    , maxOccurs_( max )
    , minValue_ ( std::numeric_limits< double >::min() )
    , maxValue_ ( std::numeric_limits< double >::max() )
    , indirectFire_( false )
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
const std::string& OrderParameter::GetName() const
{
    return name_;
}

// -----------------------------------------------------------------------------
// Name: OrderParameter::GetType
// Created: SBO 2007-04-23
// -----------------------------------------------------------------------------
const std::string& OrderParameter::GetType() const
{
    return type_;
}

// -----------------------------------------------------------------------------
// Name: OrderParameter::GetKeyName
// Created: MMC 2012-05-15
// -----------------------------------------------------------------------------
const std::string& OrderParameter::GetKeyName() const
{
    return keyName_;
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
// Name: OrderParameter::MinValue
// Created: MCO 2013-03-29
// -----------------------------------------------------------------------------
double OrderParameter::MinValue() const
{
    return minValue_;
}

// -----------------------------------------------------------------------------
// Name: OrderParameter::MaxValue
// Created: MCO 2013-03-29
// -----------------------------------------------------------------------------
double OrderParameter::MaxValue() const
{
    return maxValue_;
}

double OrderParameter::IndirectFireOnly() const
{
    return indirectFire_;
}

// -----------------------------------------------------------------------------
// Name: OrderParameter::SetName
// Created: ABR 2012-01-09
// -----------------------------------------------------------------------------
void OrderParameter::SetName( const std::string& name )
{
    name_ = name;
}

// -----------------------------------------------------------------------------
// Name: OrderParameter::SetOptional
// Created: ABR 2012-01-09
// -----------------------------------------------------------------------------
void OrderParameter::SetOptional( bool optional )
{
    optional_ = optional;
}

// -----------------------------------------------------------------------------
// Name: OrderParameter::SetMinMaxOccurs
// Created: ABR 2012-01-09
// -----------------------------------------------------------------------------
void OrderParameter::SetMinMaxOccurs( unsigned int min, unsigned int max )
{
    minOccurs_ = min;
    maxOccurs_ = max;
}

// -----------------------------------------------------------------------------
// Name: OrderParameter::SetKeyName
// Created: MMC 2012-05-15
// -----------------------------------------------------------------------------
void OrderParameter::SetKeyName( const std::string& keyName )
{
    keyName_ = keyName;
}

// -----------------------------------------------------------------------------
// Name: OrderParameter::GetValue
// Created: SBO 2007-05-25
// -----------------------------------------------------------------------------
const OrderParameterValue& OrderParameter::GetValue( unsigned int id ) const
{
    CIT_OrderParameterValues it = values_.find( id );
    if( it == values_.end() )
        throw MASA_EXCEPTION( "Undefined enumeration value." ); // $$$$ SBO 2007-05-25:
    return it->second;
}

// -----------------------------------------------------------------------------
// Name: OrderParameter::FindValue
// Created: LGY 2014-02-21
// -----------------------------------------------------------------------------
const OrderParameterValue* OrderParameter::FindValue( unsigned int id ) const
{
    auto it = values_.find( id );
    if( it == values_.end() )
        return 0;
    return &it->second;
}

// -----------------------------------------------------------------------------
// Name: OrderParameter::GetChoice
// Created: RCD 2011-05-06
// -----------------------------------------------------------------------------
const std::string& OrderParameter::GetChoice( unsigned int id ) const
{
    CIT_Aliases it = aliases_.begin();
    while( it != aliases_.end() && id > 0 )
    {
        ++it;
        --id;
    }
    if( it == aliases_.end() )
        throw MASA_EXCEPTION( "Undefined location composite parameter type." );
    return *it;
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
void OrderParameter::ReadChoice( xml::xistream& xis, T_Aliases& data, bool lowerCase )
{
    std::string type;
    xis >> xml::attribute( "type", type );
    data.insert( lowerCase ? boost::algorithm::to_lower_copy( type ) : type );
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
    for( auto it = values_.begin(); it != values_.end(); ++it )
        visitor.Visit( it->second );
}

// -----------------------------------------------------------------------------
// Name: OrderParameter::Accept
// Created: LDC 2010-08-18
// -----------------------------------------------------------------------------
void OrderParameter::Accept( ChoicesVisitor_ABC& visitor ) const
{
    for( auto it = aliases_.begin(); it != aliases_.end(); ++it )
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
    if( type_ == "phaseline" || type == "phaselinelist" )
        return "phaseline";
    if( type == "direction" )
        return "heading";
    if( type == "location" && type_ == "polygon" )
        return "polygon";
    if( type == "numeric" && type_ == "integer" )
        return "integer";
    if( type == "list" || type == "string" )
        return type_;
    if( type == "location" && type_ == "locationcomposite")
        return type;
    if( type == "quantity" && type_ == "enumeration" )
        return "quantity";
    if( type == "urbanblock" )
        return "urbanknowledge";
    return "";
}

// -----------------------------------------------------------------------------
// Name: OrderParameter::HasGenObject
// Created: LGY 2012-04-20
// -----------------------------------------------------------------------------
bool OrderParameter::HasGenObject( const std::string& type ) const
{
    return genObjects_.empty() || ( genObjects_.find( type ) != genObjects_.end() );
}
