// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "clients_gui_pch.h"
#include "RasterLayer.h"
#include "clients_kernel/ModelLoaded.h"
#include "clients_kernel/Controller.h"
#include "graphics/TextureSet.h"
#include "graphics/Visitor2d.h"

using namespace gui;

// -----------------------------------------------------------------------------
// Name: RasterLayer constructor
// Created: AGE 2007-01-04
// -----------------------------------------------------------------------------
RasterLayer::RasterLayer( kernel::Controller& controller )
    : controller_( controller )
{
    controller_.Register( *this );
}

// -----------------------------------------------------------------------------
// Name: RasterLayer destructor
// Created: AGE 2007-01-04
// -----------------------------------------------------------------------------
RasterLayer::~RasterLayer()
{
    controller_.Remove( *this );
}

// -----------------------------------------------------------------------------
// Name: RasterLayer::Paint
// Created: AGE 2007-01-04
// -----------------------------------------------------------------------------
void RasterLayer::Paint( const geometry::Rectangle2f& viewport )
{
    if( GetAlpha() == 0 )
        return;
    if( ! textures_.get() && ! graphicsDirectory_.empty() )
        try
        {
            textures_.reset( new TextureSet( graphicsDirectory_ + "/usrp.texture" ) );
        }
        catch( ... )
        {
            // $$$$ AGE 2007-01-04: 
        }
    if( textures_.get() )
    {
        glColor4f( 1, 1, 1, GetAlpha() );
        Visitor2d visitor;
        textures_->Accept( visitor, 0, viewport );

        glBindTexture( GL_TEXTURE_2D, 0 );
        glDisable(GL_TEXTURE_GEN_S);
        glDisable(GL_TEXTURE_GEN_T);
    }
}

// -----------------------------------------------------------------------------
// Name: RasterLayer::NotifyUpdated
// Created: AGE 2007-01-04
// -----------------------------------------------------------------------------
void RasterLayer::NotifyUpdated( const kernel::ModelLoaded& modelLoaded )
{
    Load( modelLoaded.scipioXml_ );
}
