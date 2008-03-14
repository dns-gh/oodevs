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
    std::string xmlType;
    xis >> attribute( "type", xmlType )
        >> attribute( "can-be-maneuver-obstacle", canBeReservedObstacle_ )
        >> attribute( "can-be-developed", canBeValorized_ )
        >> attribute( "can-be-bypassed", canBeBypassed_ );
    name_ = tools::TranslateObjectType( xmlType );
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

