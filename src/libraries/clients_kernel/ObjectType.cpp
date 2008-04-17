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
#include "xeumeuleu/xml.h"
#include "tools.h"

using namespace kernel;
using namespace xml;

// -----------------------------------------------------------------------------
// Name: ObjectType constructor
// Created: AGE 2006-02-16
// -----------------------------------------------------------------------------
ObjectType::ObjectType( xistream& xis, unsigned long id )
    : id_( id )
{
    xis >> attribute( "type", type_ )
        >> attribute( "can-be-maneuver-obstacle", canBeReservedObstacle_ )
        >> attribute( "can-be-developed", canBeValorized_ )
        >> attribute( "can-be-bypassed", canBeBypassed_ )
        >> optional() >> attribute( "geometry", geometry_ );
    name_ = tools::TranslateObjectType( type_ );
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
// Name: ObjectType::GetName
// Created: AGE 2006-02-16
// -----------------------------------------------------------------------------
std::string ObjectType::GetName() const
{
    return name_;
}

// -----------------------------------------------------------------------------
// Name: ObjectType::GetType
// Created: SBO 2008-03-20
// -----------------------------------------------------------------------------
std::string ObjectType::GetType() const
{
    return type_;
}

// -----------------------------------------------------------------------------
// Name: ObjectType::GetId
// Created: AGE 2008-03-14
// -----------------------------------------------------------------------------
unsigned long ObjectType::GetId() const
{
    return id_;
}

// -----------------------------------------------------------------------------
// Name: ObjectType::CanBeReservedObstacle
// Created: AGE 2006-10-17
// -----------------------------------------------------------------------------
bool ObjectType::CanBeReservedObstacle() const
{
    return canBeReservedObstacle_;
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
    return canBeBypassed_;
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
