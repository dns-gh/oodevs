// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "dispatcher_pch.h"
#include "PluginConfig.h"
#include "xeumeuleu/xml.h"

using namespace xml;
using namespace dispatcher;

// -----------------------------------------------------------------------------
// Name: PluginConfig constructor
// Created: SBO 2007-07-24
// -----------------------------------------------------------------------------
PluginConfig::PluginConfig( xml::xistream& xis )
{
    xis >> attribute( "name", name_ )
        >> attribute( "enabled", enabled_ )
        >> list( "parameter", *this, &PluginConfig::ReadParameter );
}

// -----------------------------------------------------------------------------
// Name: PluginConfig destructor
// Created: SBO 2007-07-24
// -----------------------------------------------------------------------------
PluginConfig::~PluginConfig()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PluginConfig::ReadParameter
// Created: SBO 2007-07-24
// -----------------------------------------------------------------------------
void PluginConfig::ReadParameter( xml::xistream& xis )
{
    std::string name, value;
    xis >> attribute( "name", name )
        >> attribute( "value", value );
    parameters_[name] = value;
}

// -----------------------------------------------------------------------------
// Name: PluginConfig::GetName
// Created: SBO 2007-07-24
// -----------------------------------------------------------------------------
std::string PluginConfig::GetName() const
{
    return name_;
}

// -----------------------------------------------------------------------------
// Name: PluginConfig::IsEnabled
// Created: SBO 2007-07-24
// -----------------------------------------------------------------------------
bool PluginConfig::IsEnabled() const
{
    return enabled_;
}

// -----------------------------------------------------------------------------
// Name: PluginConfig::GetParameter
// Created: SBO 2007-07-24
// -----------------------------------------------------------------------------
std::string PluginConfig::GetParameter( const std::string& name ) const
{
    T_Parameters::const_iterator it = parameters_.find( name );
    if( it != parameters_.end() )
        return it->second;
    return "";
}
