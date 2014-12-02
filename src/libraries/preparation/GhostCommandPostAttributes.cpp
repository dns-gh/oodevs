// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#include "preparation_pch.h"
#include "GhostCommandPostAttributes.h"
#include "clients_kernel/Tools.h"
#include "clients_gui/GLOptions.h"
#include "clients_gui/GLView_ABC.h"
#include "clients_gui/PropertiesDictionary.h"
#include "clients_gui/Viewport_ABC.h"
#include <xeumeuleu/xml.h>

// -----------------------------------------------------------------------------
// Name: GhostCommandPostAttributes constructor
// Created: ABR 2012-07-09
// -----------------------------------------------------------------------------
GhostCommandPostAttributes::GhostCommandPostAttributes( const kernel::Entity_ABC& entity, bool isCommandPost, gui::PropertiesDictionary& dictionary, const std::string& symbol )
    : entity_( entity )
    , symbol_( symbol )
    , isPc_( isCommandPost )
{
    CreateDictionary( dictionary );
}

// -----------------------------------------------------------------------------
// Name: GhostCommandPostAttributes constructor
// Created: ABR 2012-07-09
// -----------------------------------------------------------------------------
GhostCommandPostAttributes::GhostCommandPostAttributes( const kernel::Entity_ABC& entity, xml::xistream& xis, gui::PropertiesDictionary& dictionary, const std::string& symbol )
    : entity_( entity )
    , symbol_( symbol )
    , isPc_( xis.attribute< bool >( "command-post", false ) )
{
    CreateDictionary( dictionary );
}

// -----------------------------------------------------------------------------
// Name: GhostCommandPostAttributes destructor
// Created: ABR 2012-07-09
// -----------------------------------------------------------------------------
GhostCommandPostAttributes::~GhostCommandPostAttributes()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: GhostCommandPostAttributes::IsCommandPost
// Created: ABR 2012-07-09
// -----------------------------------------------------------------------------
bool GhostCommandPostAttributes::IsCommandPost() const
{
    return isPc_;
}

// -----------------------------------------------------------------------------
// Name: GhostCommandPostAttributes::CreateDictionary
// Created: ABR 2012-07-09
// -----------------------------------------------------------------------------
void GhostCommandPostAttributes::CreateDictionary( gui::PropertiesDictionary& dictionary )
{
    dictionary.Register( entity_, tools::translate( "Agent", "Info/Command post" ), isPc_, *this, &GhostCommandPostAttributes::Update, false );
}

// -----------------------------------------------------------------------------
// Name: GhostCommandPostAttributes::Draw
// Created: JSR 2014-12-01
// -----------------------------------------------------------------------------
void GhostCommandPostAttributes::Draw( const geometry::Point2f& where,
                                       const gui::Viewport_ABC& viewport,
                                       gui::GLView_ABC& tools ) const
{
    if( isPc_ && !entity_.IsAnAggregatedSubordinate() && viewport.IsHotpointVisible() )
        tools.DrawApp6SymbolFixedSize( symbol_, where, -tools.GetCurrentOptions().GetRatio( entity_ ), 0 );
}

// -----------------------------------------------------------------------------
// Name: GhostCommandPostAttributes::Update
// Created: ABR 2012-07-09
// -----------------------------------------------------------------------------
void GhostCommandPostAttributes::Update( const bool& commandPost )
{
    isPc_ = commandPost;
}

// -----------------------------------------------------------------------------
// Name: GhostCommandPostAttributes::SerializeAttributes
// Created: ABR 2012-07-09
// -----------------------------------------------------------------------------
void GhostCommandPostAttributes::SerializeAttributes( xml::xostream& xos ) const
{
    xos << xml::attribute( "command-post", isPc_ );
}
