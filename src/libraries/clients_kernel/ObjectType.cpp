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
#include "Location_ABC.h"
#include <xeumeuleu/xml.hpp>

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: ObjectType constructor
// Created: AGE 2006-02-16
// -----------------------------------------------------------------------------
ObjectType::ObjectType( xml::xistream& xis )
    : name_          ( xis.attribute< std::string >( "name" ) )
    , type_          ( xis.attribute< std::string >( "type" ) )
    , description_   ( xis.attribute< std::string >( "description", "" ) )
    , canBeValorized_( false )
    , canBeBypassed_ ( false )
    , pointSize_     ( 0 )
{
    xis >> xml::optional >> xml::attribute( "point-size", pointSize_ )
        >> xml::optional >> xml::start( "geometries" )
            >> xml::list( "geometry", *this, &ObjectType::ReadGeometry )
        >> xml::end
        >> xml::optional >> xml::start( "constructor" )
            >> xml::list( "improvable", *this, &ObjectType::SetValorizable )
        >> xml::end
        >> xml::optional
        >> xml::start( "disaster" )
            >> xml::attribute( "model", disaster_ )
        >> xml::end;
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
// Name: ObjectType::ReadGeometry
// Created: JSR 2012-02-17
// -----------------------------------------------------------------------------
void ObjectType::ReadGeometry( xml::xistream& xis )
{
    geometrySymbols_[ xis.attribute< std::string >( "type" ) ] = xis.attribute< std::string >( "symbol" );
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
const std::string& ObjectType::GetSymbol( const std::string& locationType ) const
{
    static const std::string emptyString;
    CIT_GeometrySymbols it = geometrySymbols_.find( locationType );
    if( it != geometrySymbols_.end() )
        return it->second;
    return emptyString;
}

// -----------------------------------------------------------------------------
// Name: ObjectType::GetDescription
// Created: JSR 2011-11-21
// -----------------------------------------------------------------------------
const std::string& ObjectType::GetDescription() const
{
    return description_;
}

// -----------------------------------------------------------------------------
// Name: ObjectType::GetDisasterType
// Created: LGY 2012-11-21
// -----------------------------------------------------------------------------
const std::string& ObjectType::GetDisasterType() const
{
    return disaster_;
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
            std::string density = xis.attribute< std::string >( "unit-type" );
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
    return capacities_.find( "logistic" ) != capacities_.end();
}

// -----------------------------------------------------------------------------
// Name: ObjectType::HasSpawn
// Created: JSR 2010-06-02
// -----------------------------------------------------------------------------
bool ObjectType::HasSpawn() const
{
    return capacities_.find( "spawn" ) != capacities_.end();
}

// -----------------------------------------------------------------------------
// Name: ObjectType::HasMedicalCapacity
// Created: JCR 2010-06-06
// -----------------------------------------------------------------------------
bool ObjectType::HasMedicalCapacity() const
{
    return capacities_.find( "medical" ) != capacities_.end();
}

// -----------------------------------------------------------------------------
// Name: ObjectType::HasAltitudeModifierCapacity
// Created: MMC 2012-04-19
// -----------------------------------------------------------------------------
bool ObjectType::HasAltitudeModifierCapacity() const
{
    return capacities_.find( "altitude-modifier" ) != capacities_.end();
}

// -----------------------------------------------------------------------------
// Name: ObjectType::HasTimeLimitedCapacity
// Created: MMC 2012-04-19
// -----------------------------------------------------------------------------
bool ObjectType::HasTimeLimitedCapacity() const
{
    return capacities_.find( "time-limited" ) != capacities_.end();
}

// -----------------------------------------------------------------------------
// Name: ObjectType::HasLodgingCapacity
// Created: MMC 2012-07-03
// -----------------------------------------------------------------------------
bool ObjectType::HasLodgingCapacity() const
{
    return capacities_.find( "lodging" ) != capacities_.end();
}

// -----------------------------------------------------------------------------
// Name: ObjectType::HasResourceNetwork
// Created: JSR 2013-06-07
// -----------------------------------------------------------------------------
bool ObjectType::HasResourceNetwork() const
{
    return capacities_.find( "resources" ) != capacities_.end();
}

// -----------------------------------------------------------------------------
// Name: ObjectType::HasBurnCapacity
// Created: LGY 2013-11-26
// -----------------------------------------------------------------------------
bool ObjectType::HasBurnCapacity() const
{
    return capacities_.find( "burn" ) != capacities_.end();
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
// Name: ObjectType::CanBeActivated
// Created: AGE 2006-10-17
// -----------------------------------------------------------------------------
bool ObjectType::CanBeActivated() const
{
    return capacities_.find( "activable" ) != capacities_.end();
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
// Name: ObjectType::CanBeTrafficable
// Created: LGY 2011-10-25
// -----------------------------------------------------------------------------
bool ObjectType::CanBeTrafficable() const
{
    return capacities_.find( "trafficability" ) != capacities_.end();
}

// -----------------------------------------------------------------------------
// Name: ObjectType::CanBePoint
// Created: SBO 2008-04-17
// -----------------------------------------------------------------------------
bool ObjectType::CanBePoint() const
{
    return geometrySymbols_.find( "point" ) != geometrySymbols_.end();
}

// -----------------------------------------------------------------------------
// Name: ObjectType::CanBeLine
// Created: SBO 2008-04-17
// -----------------------------------------------------------------------------
bool ObjectType::CanBeLine() const
{
    return geometrySymbols_.find( "line" ) != geometrySymbols_.end();
}

// -----------------------------------------------------------------------------
// Name: ObjectType::CanBeRectangle
// Created: SLG 2010-03-25
// -----------------------------------------------------------------------------
bool ObjectType::CanBeRectangle() const
{
    return geometrySymbols_.find( "rectangle" ) != geometrySymbols_.end();
}

// -----------------------------------------------------------------------------
// Name: ObjectType::CanBePolygon
// Created: SBO 2008-04-17
// -----------------------------------------------------------------------------
bool ObjectType::CanBePolygon() const
{
    return geometrySymbols_.find( "polygon" ) != geometrySymbols_.end();
}

// -----------------------------------------------------------------------------
// Name: ObjectType::CanBeCircle
// Created: SBO 2008-04-17
// -----------------------------------------------------------------------------
bool ObjectType::CanBeCircle() const
{
    return geometrySymbols_.find( "circle" ) != geometrySymbols_.end();
}

float ObjectType::GetPointSize() const
{
    return pointSize_;
}
