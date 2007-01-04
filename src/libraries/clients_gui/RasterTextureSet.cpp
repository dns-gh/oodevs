// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "clients_gui_pch.h"
#include "RasterTextureSet.h"
#include "clients_kernel/Controller.h"
#include "clients_kernel/ModelLoaded.h"

using namespace gui;

// -----------------------------------------------------------------------------
// Name: RasterTextureSet constructor
// Created: AGE 2007-01-04
// -----------------------------------------------------------------------------
RasterTextureSet::RasterTextureSet( kernel::Controller& controller )
    : controller_( controller )
    , loaded_( false )
{
    controller_.Register( *this );
}

// -----------------------------------------------------------------------------
// Name: RasterTextureSet destructor
// Created: AGE 2007-01-04
// -----------------------------------------------------------------------------
RasterTextureSet::~RasterTextureSet()
{
    controller_.Remove( *this );
}

// -----------------------------------------------------------------------------
// Name: RasterTextureSet::Accept
// Created: AGE 2007-01-04
// -----------------------------------------------------------------------------
void RasterTextureSet::Accept( TextureVisitor_ABC& visitor, unsigned level, const geometry::Rectangle2f& extent )
{
    if( ! loaded_ && ! graphicsDirectory_.empty() )
    {
        try 
        {
            TextureSet::Load( graphicsDirectory_ + "/usrp.texture" );
        }
        catch( ... )
        {
        }
    }
    TextureSet::Accept( visitor, level, extent );
}

// -----------------------------------------------------------------------------
// Name: RasterTextureSet::NotifyUpdated
// Created: AGE 2007-01-04
// -----------------------------------------------------------------------------
void RasterTextureSet::NotifyUpdated( const kernel::ModelLoaded& modelLoaded )
{
    loaded_ = false;
    Clear();
    kernel::WorldParameters::Load( modelLoaded.scipioXml_ );
}
