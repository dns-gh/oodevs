// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#include "gaming_app_pch.h"
#include "MissionTemplate.h"
#include "MissionParameter.h"
#include "xeumeuleu/xml.h"

using namespace xml;

// -----------------------------------------------------------------------------
// Name: MissionTemplate constructor
// Created: SBO 2006-11-22
// -----------------------------------------------------------------------------
MissionTemplate::MissionTemplate( xml::xistream& xis )
{
    std::string name, diaType, diaBehavior;
    xis >> attribute( "id", (int&)id_ )
        >> attribute( "name", name )
        >> list( "parameter", *this, &MissionTemplate::ReadParameter );
    name_ = name.c_str();
}

// -----------------------------------------------------------------------------
// Name: MissionTemplate destructor
// Created: SBO 2006-11-22
// -----------------------------------------------------------------------------
MissionTemplate::~MissionTemplate()
{
    for( CIT_Parameters it = parameters_.begin(); it != parameters_.end(); ++it )
        delete *it;
}

// -----------------------------------------------------------------------------
// Name: MissionTemplate::ReadParameter
// Created: SBO 2006-11-22
// -----------------------------------------------------------------------------
void MissionTemplate::ReadParameter( xml::xistream& xis )
{
    MissionParameter* param = new MissionParameter( xis );
    parameters_.push_back( param );
}

// -----------------------------------------------------------------------------
// Name: MissionTemplate::GetId
// Created: SBO 2006-11-22
// -----------------------------------------------------------------------------
unsigned long MissionTemplate::GetId() const
{
    return id_;
}

// -----------------------------------------------------------------------------
// Name: MissionTemplate::BuildInterface
// Created: SBO 2006-11-22
// -----------------------------------------------------------------------------
void MissionTemplate::BuildInterface( MissionInterfaceBuilder& builder ) const
{
    for( CIT_Parameters it = parameters_.begin(); it != parameters_.end(); ++it )
        (*it)->BuildInterface( builder );
}
