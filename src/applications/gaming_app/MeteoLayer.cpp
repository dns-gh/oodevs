// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_app_pch.h"
#include "MeteoLayer.h"
#include "gaming/AmmoEffect.h"
#include "clients_kernel/Controllers.h"
#include "clients_kernel/GlTools_ABC.h"

using namespace kernel;
using namespace gui;

// -----------------------------------------------------------------------------
// Name: MeteoLayer constructor
// Created: AGE 2006-04-04
// -----------------------------------------------------------------------------
MeteoLayer::MeteoLayer( Controllers& controllers, const GlTools_ABC& tools )
    : controllers_( controllers )
    , tools_( tools )
{
    controllers_.Register( *this );
}

// -----------------------------------------------------------------------------
// Name: MeteoLayer destructor
// Created: AGE 2006-04-04
// -----------------------------------------------------------------------------
MeteoLayer::~MeteoLayer()
{
    controllers_.Remove( *this );
}

// -----------------------------------------------------------------------------
// Name: MeteoLayer::Paint
// Created: AGE 2006-04-04
// -----------------------------------------------------------------------------
void MeteoLayer::Paint( const geometry::Rectangle2f& )
{
    glPushAttrib( GL_CURRENT_BIT );
    for( IT_Effects it = effects_.begin(); it != effects_.end(); ++it )
        (*it)->Draw( tools_ );
    glPopAttrib();
}

// -----------------------------------------------------------------------------
// Name: MeteoLayer::NotifyCreated
// Created: AGE 2006-04-04
// -----------------------------------------------------------------------------
void MeteoLayer::NotifyCreated( const AmmoEffect& effect )
{
    effects_.push_back( & effect );
}

// -----------------------------------------------------------------------------
// Name: MeteoLayer::NotifyDeleted
// Created: AGE 2006-04-04
// -----------------------------------------------------------------------------
void MeteoLayer::NotifyDeleted( const AmmoEffect& effect )
{
    IT_Effects it = std::find( effects_.begin(), effects_.end(), &effect );
    if( it != effects_.end() )
    {
        std::swap( *it, effects_.back() );
        effects_.pop_back();
    }
}
