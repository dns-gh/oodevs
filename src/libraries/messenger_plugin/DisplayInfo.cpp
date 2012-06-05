// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "messenger_plugin_pch.h"
#include "DisplayInfo.h"

#include <xeumeuleu/xml.hpp>

using namespace plugins::messenger;

namespace
{
    std::string empty;
}

// -----------------------------------------------------------------------------
// Name: DisplayInfo constructor
// Created: LDC 2012-06-05
// -----------------------------------------------------------------------------
DisplayInfo::DisplayInfo( xml::xistream& xis )
    : color_ ( xis.attribute( "color", empty ) )
    , level_ ( xis.attribute( "level", empty ) )
{
        // NOTHING
}

// -----------------------------------------------------------------------------
// Name: DisplayInfo::Color
// Created: LDC 2012-06-05
// -----------------------------------------------------------------------------
std::string DisplayInfo::Color() const
{
    return color_;
}
    
// -----------------------------------------------------------------------------
// Name: DisplayInfo::Level
// Created: LDC 2012-06-05
// -----------------------------------------------------------------------------
std::string DisplayInfo::Level() const
{
    return level_;
}
