// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_app_pch.h"
#include "TextLayer.h"
#include "GlButton.h"
#include "gaming/CommandPublisher.h"
#include "gaming/CommandHandler.h"
#include "gaming/Command.h"
#include "clients_kernel/GlTools_ABC.h"
#include "clients_kernel/GlTooltip_ABC.h"
#include "clients_kernel/Displayer_ABC.h"
#include "clients_gui/GlTooltip.h"
#include <boost/bind.hpp>

// -----------------------------------------------------------------------------
// Name: TextLayer constructor
// Created: AGE 2008-06-24
// -----------------------------------------------------------------------------
TextLayer::TextLayer( kernel::Controllers& controllers, Publisher_ABC& publisher, const kernel::GlTools_ABC& tools, CommandHandler& handler )
    : tools_  ( tools )
    , handler_( handler )
    , publisher_( new CommandPublisher( controllers, publisher ) )
{
    handler_.Register( "display", *this );
    handler_.Register( "prompt", *this );
}

// -----------------------------------------------------------------------------
// Name: TextLayer destructor
// Created: AGE 2008-06-24
// -----------------------------------------------------------------------------
TextLayer::~TextLayer()
{
    handler_.Unregister( "prompt", *this );
    handler_.Unregister( "display", *this );
}

// -----------------------------------------------------------------------------
// Name: TextLayer::Paint
// Created: AGE 2008-06-24
// -----------------------------------------------------------------------------
void TextLayer::Paint( const ViewFrustum& frustum )
{
    // $$$$ AGE 2008-06-24: 
}

// -----------------------------------------------------------------------------
// Name: TextLayer::Paint
// Created: AGE 2008-06-24
// -----------------------------------------------------------------------------
void TextLayer::Paint( const geometry::Rectangle2f& viewport )
{
    if( !ShouldDrawPass() || !tooltip_.get() )
        return;
    geometry::Point2f topLeft( viewport.Left(), viewport.Top() );
    geometry::Vector2f offset( viewport.Width(), -viewport.Height() );
    topLeft += offset * 0.001f;
    tooltip_->Draw( topLeft, viewport.Width() / tools_.Pixels(), -50, tools_.Pixels() );
    gui::GlTooltip* tooltip = (gui::GlTooltip*)( tooltip_.get() );
    if( button_.get() )
    {
        geometry::Rectangle2f boundingBox( viewport.Left(), viewport.Top()
                                         , viewport.Left() + tooltip->Size().width() * tools_.Pixels()
                                         , viewport.Top()  - tooltip->Size().height() * tools_.Pixels() );
        button_->Draw( boundingBox );
    }
}

// -----------------------------------------------------------------------------
// Name: TextLayer::Receive
// Created: AGE 2008-06-24
// -----------------------------------------------------------------------------
void TextLayer::Receive( const Command& command )
{
    if( command.ArgumentCount() == 0 )
    {
        tooltip_.reset();
        button_.reset();
        return;
    }
    if( command.Name() == "display" )
        Display( command.Argument( 1 ).c_str() );
    else if( command.Name() == "prompt" && command.ArgumentCount() > 2 )
    {
        currentPrompt_ = command.Argument( 1 );
        Display( command.Argument( 2 ).c_str() );
        const QStringList buttons = QStringList::split( '|', command.Argument( 3 ).c_str() );
        for( QStringList::const_iterator it = buttons.begin(); it != buttons.end(); ++it )
            AddButton( *it ); // $$$$ SBO 2008-07-07: only handle one button, handle a button stack if needed
    }
}

// -----------------------------------------------------------------------------
// Name: TextLayer::HandleMousePress
// Created: SBO 2008-07-04
// -----------------------------------------------------------------------------
bool TextLayer::HandleMousePress( QMouseEvent* mouse, const geometry::Point2f& point )
{
    if( mouse->button() & Qt::LeftButton && button_.get() && button_->HandleMousePress( point ) )
    {
        if( mouse->state() != Qt::NoButton )
        {
            const std::string message = "/choose " + currentPrompt_ + " \"" + button_->Label().ascii() + "\"";
            publisher_->Send( "", message );
            button_.reset();
        }
        return true;
    }
    return false;
}

// -----------------------------------------------------------------------------
// Name: TextLayer::HandleMouseMove
// Created: SBO 2008-07-07
// -----------------------------------------------------------------------------
bool TextLayer::HandleMouseMove( QMouseEvent*, const geometry::Point2f& point )
{
    if( button_.get() )
        return button_->HandleMouseMove( point );
    return false;
}

// -----------------------------------------------------------------------------
// Name: TextLayer::Display
// Created: SBO 2008-07-07
// -----------------------------------------------------------------------------
void TextLayer::Display( const QString& text )
{
    button_.reset();
    tooltip_ = tools_.CreateTooltip();
    {
        gui::GlTooltip* tooltip = (gui::GlTooltip*)( tooltip_.get() );
        tooltip->SetFrameDrawer( boost::bind( &TextLayer::DrawFrame, this, _1, _2 ) );
    }
    kernel::Displayer_ABC& display = *tooltip_;
    display.Start( QFont( "Arial", 12, QFont::Bold ) )
                .Add( Qt::white )
                .Add( text )
           .End();
}

// -----------------------------------------------------------------------------
// Name: TextLayer::AddButton
// Created: SBO 2008-07-04
// -----------------------------------------------------------------------------
void TextLayer::AddButton( const QString& label )
{
    button_.reset( new GlButton( label, tools_ ) );
    button_->SetPosition( Qt::AlignRight );
    button_->SetMargin( 10, 10 );
}

// -----------------------------------------------------------------------------
// Name: TextLayer::DrawFrame
// Created: SBO 2008-07-21
// -----------------------------------------------------------------------------
void TextLayer::DrawFrame( QPainter& painter, const QRect& rect )
{
    painter.fillRect( rect, Qt::black );
    painter.drawImage( rect, QImage( "resources/images/gaming/tooltipmask.png" ) );
    painter.setPen( QColor( 159, 174, 216 ) );
    painter.drawRect( rect );
}
