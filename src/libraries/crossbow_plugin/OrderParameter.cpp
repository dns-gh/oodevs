// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "crossbow_plugin_pch.h"
#include "OrderParameter.h"
#include "game_asn/Simulation.h"
#include "xeumeuleu/xml.h"
#include <algorithm>

using namespace xml;
using namespace kernel;

// -----------------------------------------------------------------------------
// Name: OrderParameter constructor
// Created: SBO 2007-05-31
// -----------------------------------------------------------------------------
OrderParameter::OrderParameter( xml::xistream& xis )
{
    xis >> attribute( "name", name_ )
        >> attribute( "type", type_ )
        >> attribute( "optional", optional_ );
    SetTypeId();
}

// -----------------------------------------------------------------------------
// Name: OrderParameter destructor
// Created: SBO 2007-05-31
// -----------------------------------------------------------------------------
OrderParameter::~OrderParameter()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: OrderParameter::GetName
// Created: SBO 2007-05-31
// -----------------------------------------------------------------------------
std::string OrderParameter::GetName() const
{
    return name_;
}

// -----------------------------------------------------------------------------
// Name: OrderParameter::GetTypeId
// Created: SBO 2007-05-31
// -----------------------------------------------------------------------------
unsigned int OrderParameter::GetTypeId() const
{
    return typeId_;
}

// -----------------------------------------------------------------------------
// Name: OrderParameter::SetTypeId
// Created: SBO 2007-05-31
// -----------------------------------------------------------------------------
void OrderParameter::SetTypeId()
{
    std::string lowerType = type_;
    std::transform( type_.begin(), type_.end(), lowerType.begin(), &tolower );
    typeId_ = 0;
    if( lowerType == "location" )
        typeId_ = T_MissionParameter_value_location;
    else if( lowerType == "point" )
        typeId_ = T_MissionParameter_value_point;
    else if( lowerType == "polygon" )
        typeId_ = T_MissionParameter_value_polygon;
    else if( lowerType == "path" )
        typeId_ = T_MissionParameter_value_path;
    else if( lowerType == "bool" )
        typeId_ = T_MissionParameter_value_aBool;
    else if( lowerType == "automate" ) // $$$$ SBO 2007-07-24: 
        typeId_ = T_MissionParameter_value_automat;
}
