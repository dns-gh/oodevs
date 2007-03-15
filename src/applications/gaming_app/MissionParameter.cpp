// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_app_pch.h"
#include "MissionParameter.h"
#include "MissionInterfaceBuilder.h"
#include "xeumeuleu/xml.h"

using namespace xml;

// -----------------------------------------------------------------------------
// Name: MissionParameter constructor
// Created: SBO 2006-11-22
// -----------------------------------------------------------------------------
MissionParameter::MissionParameter( xml::xistream& xis )
    : optional_( false )
{
    std::string name, diaName, type;
    xis >> attribute( "name", name )
        >> attribute( "dia-name", diaName )
        >> attribute( "type", type )
        >> optional() >> attribute( "optional", optional_ )
        >> list( "value", *this, ReadValue );
    name_ = name.c_str();
    diaName_ = diaName.c_str();
    type_ = type.c_str();
}
    
// -----------------------------------------------------------------------------
// Name: MissionParameter destructor
// Created: SBO 2006-11-22
// -----------------------------------------------------------------------------
MissionParameter::~MissionParameter()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MissionParameter::GetName
// Created: SBO 2006-11-22
// -----------------------------------------------------------------------------
QString MissionParameter::GetName() const
{
    return name_;
}

// -----------------------------------------------------------------------------
// Name: MissionParameter::ReadValue
// Created: SBO 2006-12-01
// -----------------------------------------------------------------------------
void MissionParameter::ReadValue( xml::xistream& xis )
{
    int id;
    std::string name;
    xis >> attribute( "id", id )
        >> attribute( "name", name );
    values_.push_back( std::make_pair( id, name ) );
}

// -----------------------------------------------------------------------------
// Name: MissionParameter::BuildInterface
// Created: SBO 2006-11-22
// -----------------------------------------------------------------------------
void MissionParameter::BuildInterface( MissionInterfaceBuilder& builder ) const
{
    builder.AddParameter( type_, name_, optional_, values_ );
}
