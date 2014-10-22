// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#include "clients_gui_pch.h"
#include "ContourLinesLayer.h"
#include "ContourLinesComputer.h"
#include "GLView_ABC.h"
#include "GLOptions.h"
#include "clients_kernel/Controllers.h"
#include "clients_kernel/OptionVariant.h"

using namespace gui;

// -----------------------------------------------------------------------------
// Name: ContourLinesLayer constructor
// Created: SBO 2010-03-23
// -----------------------------------------------------------------------------
ContourLinesLayer::ContourLinesLayer( kernel::Controllers& controllers,
                                      GLView_ABC& view )
    : Layer2D( controllers, view, eLayerTypes_ContourLines )
    , modelLoaded_( false )
{
    controllers_.controller_.Update( *this );
}

// -----------------------------------------------------------------------------
// Name: ContourLinesLayer destructor
// Created: SBO 2010-03-23
// -----------------------------------------------------------------------------
ContourLinesLayer::~ContourLinesLayer()
{
    controllers_.controller_.Unregister( *this );
}

// -----------------------------------------------------------------------------
// Name: ContourLinesLayer::NotifyUpdated
// Created: SBO 2006-05-24
// -----------------------------------------------------------------------------
void ContourLinesLayer::NotifyUpdated( const kernel::ModelLoaded& )
{
    modelLoaded_ = true;
}

// -----------------------------------------------------------------------------
// Name: ContourLinesLayer::NotifyUpdated
// Created: ABR 2014-07-02
// -----------------------------------------------------------------------------
void ContourLinesLayer::NotifyUpdated( const kernel::ModelUnLoaded& )
{
    modelLoaded_ = false;
}

// -----------------------------------------------------------------------------
// Name: ContourLinesLayer::Paint
// Created: AGE 2006-03-29
// -----------------------------------------------------------------------------
void ContourLinesLayer::Paint( const geometry::Rectangle2f& )
{
    auto& options = view_.GetOptions();
    if( !modelLoaded_ ||
        !ShouldDrawPass() ||
        GetAlpha() == 0 ||
        !options.Get( "ContourLines/Enabled" ).To< bool >() )
        return;
    auto& computer = options.GetContourLinesComputer();
    if( !computer )
        return;
    auto callList = computer->CreateCallList();
    if( callList == 0 )
        return;
    const QColor color( options.Get( "ContourLines/Color" ).To< QString >() );
    glPushAttrib( GL_LINE_BIT | GL_CURRENT_BIT | GL_STENCIL_BUFFER_BIT );
        glColor4f( color.red() / 256.f, color.green() / 256.f, color.blue() / 256.f, GetAlpha() );
        glEnable( GL_LINE_SMOOTH );
        glCallList( callList );
    glPopAttrib();
}
