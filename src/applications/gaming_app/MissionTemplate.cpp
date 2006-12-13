// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
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
void MissionTemplate::BuildInterface( MissionInterfaceBuilder& builder, ASN1T_MissionParameters& mission ) const
{
    mission.n = parameters_.size();
    mission.elem = new ASN1T_MissionParameter[mission.n]; // $$$$ SBO 2006-11-22: 
    for( unsigned int i = 0; i < parameters_.size(); ++i )
        parameters_[i]->BuildInterface( builder, mission.elem[i] );
}
