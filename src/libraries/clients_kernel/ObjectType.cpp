// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "clients_kernel_pch.h"
#include "ObjectType.h"
#include <xeumeuleu/xml.hpp>

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: ObjectType constructor
// Created: AGE 2006-02-16
// -----------------------------------------------------------------------------
ObjectType::ObjectType( xml::xistream& xis )
    : name_( xis.attribute< std::string >( "name" ) )
    , type_( xis.attribute< std::string >( "type" ) )
    , symbol_ ( xis.attribute< std::string >( "symbol", "" ) )
    , geometry_( xis.attribute< std::string >( "geometry", "" ) )
    , canBeValorized_ ( false )
    , canBeBypassed_ ( false )
{
    xis >> xml::optional() >> xml::start( "constructor" )
            >> xml::list( "improvable", *this, &ObjectType::SetValorizable )
        >> xml::end();
    xis >> xml::list( *this, &ObjectType::ReadCapacities );
}

// -----------------------------------------------------------------------------
// Name: ObjectType destructor
// Created: AGE 2006-02-16
// -----------------------------------------------------------------------------
ObjectType::~ObjectType()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ObjectType::SetValorizable
// Created: LDC 2010-12-27
// -----------------------------------------------------------------------------
void ObjectType::SetValorizable( xml::xistream& /*xis*/ )
{
    canBeValorized_ = true;
}

// -----------------------------------------------------------------------------
// Name: ObjectType::ReadAttributes
// Created: JCR 2008-06-10
// -----------------------------------------------------------------------------
void ObjectType::ReadCapacities( const std::string& capacity, xml::xistream& xis )
{
    capacities_[ capacity ].reset( new xml::xibufferstream( xis ) );
}

// -----------------------------------------------------------------------------
// Name: ObjectType::GetName
// Created: AGE 2006-02-16
// -----------------------------------------------------------------------------
const std::string& ObjectType::GetName() const
{
    return name_;
}

// -----------------------------------------------------------------------------
// Name: ObjectType::GetType
// Created: SBO 2008-03-20
// -----------------------------------------------------------------------------
const std::string& ObjectType::GetType() const
{
    return type_;
}

// -----------------------------------------------------------------------------
// Name: ObjectType::GetSymbol
// Created: JCR 2008-06-10
// -----------------------------------------------------------------------------
const std::string& ObjectType::GetSymbol() const
{
    return symbol_;
}

// -----------------------------------------------------------------------------
// Name: ObjectType::CapacitiesBegin
// Created: LDC 2009-03-17
// -----------------------------------------------------------------------------
ObjectType::CIT_Capacities ObjectType::CapacitiesBegin() const
{
    return capacities_.begin();
}

// -----------------------------------------------------------------------------
// Name: ObjectType::CapacitiesEnd
// Created: LDC 2009-03-17
// -----------------------------------------------------------------------------
ObjectType::CIT_Capacities ObjectType::CapacitiesEnd() const
{
    return capacities_.end();
}

// -----------------------------------------------------------------------------
// Name: ObjectType::FindCapacity
// Created: LDC 2009-03-17
// -----------------------------------------------------------------------------
xml::xistream* ObjectType::FindCapacity( const std::string& capacity ) const
{
    CIT_Capacities it = capacities_.find( capacity );
    if( it != capacities_.end() )
        return it->second.get();
    return 0;
}

// -----------------------------------------------------------------------------
// Name: ObjectType::HasBuildableDensity
// Created: LDC 2009-03-17
// -----------------------------------------------------------------------------
bool ObjectType::HasBuildableDensity() const
{
    xml::xistream* pXis = FindCapacity( "constructor" );
    if( pXis )
    {
        xml::xistream& xis = *pXis;
        if( xis.has_child( "buildable" ) && xis.has_attribute( "unit-type" ) )
        {
            std::string density;
            xis >> xml::attribute( "unit-type", density );
            xis >> xml::start( "buildable" );
            bool hasResources = xis.has_child( "resources" );
            xis >> xml::end;
            return( hasResources && density == "density" );
        }
    }
    return false;
}

// -----------------------------------------------------------------------------
// Name: ObjectType::HasLogistic
// Created: LDC 2009-03-17
// -----------------------------------------------------------------------------
bool ObjectType::HasLogistic() const
{
    return( capacities_.find( "logistic" ) != capacities_.end() );
}

// -----------------------------------------------------------------------------
// Name: ObjectType::HasSpawn
// Created: JSR 2010-06-02
// -----------------------------------------------------------------------------
bool ObjectType::HasSpawn() const
{
    return( capacities_.find( "spawn" ) != capacities_.end() );
}

// -----------------------------------------------------------------------------
// Name: ObjectType::HasMedicalCapacity
// Created: JCR 2010-06-06
// -----------------------------------------------------------------------------
bool ObjectType::HasMedicalCapacity() const
{
    return( capacities_.find( "medical" ) != capacities_.end() );
}

// -----------------------------------------------------------------------------
// Name: ObjectType::IsUrban
// Created: SLG 2011-01-06
// -----------------------------------------------------------------------------
bool ObjectType::IsUrban() const
{
    return type_ == "urban block";
}

// -----------------------------------------------------------------------------
// Name: ObjectType::CanBeReservedObstacle
// Created: AGE 2006-10-17
// -----------------------------------------------------------------------------
bool ObjectType::CanBeReservedObstacle() const
{
    return( capacities_.find( "activable" ) != capacities_.end() );
}

// -----------------------------------------------------------------------------
// Name: ObjectType::CanBeValorized
// Created: AGE 2006-10-17
// -----------------------------------------------------------------------------
bool ObjectType::CanBeValorized() const
{
    return canBeValorized_;
}

// -----------------------------------------------------------------------------
// Name: ObjectType::CanBeBypassed
// Created: AGE 2006-10-17
// -----------------------------------------------------------------------------
bool ObjectType::CanBeBypassed() const
{
    return capacities_.find( "bypassable" ) != capacities_.end();
}

// -----------------------------------------------------------------------------
// Name: ObjectType::CanBePoint
// Created: SBO 2008-04-17
// -----------------------------------------------------------------------------
bool ObjectType::CanBePoint() const
{
    return geometry_.empty() || geometry_.find( "point" ) != std::string::npos;
}

// -----------------------------------------------------------------------------
// Name: ObjectType::CanBeLine
// Created: SBO 2008-04-17
// -----------------------------------------------------------------------------
bool ObjectType::CanBeLine() const
{
    return geometry_.empty() || geometry_.find( "line" ) != std::string::npos;
}

// -----------------------------------------------------------------------------
// Name: ObjectType::CanBeRectangle
// Created: SLG 2010-03-25
// -----------------------------------------------------------------------------
bool ObjectType::CanBeRectangle() const
{
    return geometry_.empty() || geometry_.find( "rectangle" ) != std::string::npos;
}

// -----------------------------------------------------------------------------
// Name: ObjectType::CanBePolygon
// Created: SBO 2008-04-17
// -----------------------------------------------------------------------------
bool ObjectType::CanBePolygon() const
{
    return geometry_.empty() || geometry_.find( "polygon" ) != std::string::npos;
}

// -----------------------------------------------------------------------------
// Name: ObjectType::CanBeCircle
// Created: SBO 2008-04-17
// -----------------------------------------------------------------------------
bool ObjectType::CanBeCircle() const
{
    return geometry_.empty() || geometry_.find( "circle" ) != std::string::npos;
}
