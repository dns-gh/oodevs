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
    ClearButtons();
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
    geometry::Rectangle2f boundingBox( viewport.Left(), viewport.Top()
                                     , viewport.Left() + tooltip->Size().width() * tools_.Pixels()
                                     , viewport.Top()  - tooltip->Size().height() * tools_.Pixels() );
    for( T_Buttons::const_reverse_iterator it = buttons_.rbegin(); it != buttons_.rend(); ++it )
    {
        (*it)->Draw( boundingBox );
        const geometry::Rectangle2f box = (*it)->BoundingBox();
        boundingBox.Set( boundingBox.Left(), boundingBox.Bottom(), std::min( boundingBox.Right(), box.Right() ), boundingBox.Top() );
        // $$$$ SBO 2009-02-11: only works for right align
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
        ClearButtons();
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
            AddButton( *it );
    }
}

// -----------------------------------------------------------------------------
// Name: TextLayer::HandleMousePress
// Created: SBO 2008-07-04
// -----------------------------------------------------------------------------
bool TextLayer::HandleMousePress( QMouseEvent* mouse, const geometry::Point2f& point )
{
    if( mouse->button() & Qt::LeftButton )
    {
        for( T_Buttons::const_iterator it = buttons_.begin(); it != buttons_.end(); ++it )
        {
            if( (*it)->HandleMousePress( point ) )
            {
                if( mouse->state() != Qt::NoButton )
                {
                    const std::string message = "/choose " + currentPrompt_ + " \"" + (*it)->Label().ascii() + "\"";
                    publisher_->Send( "", message );
                    ClearButtons();
                }
                return true;
            }
        }
    }
    return false;
}

// -----------------------------------------------------------------------------
// Name: TextLayer::HandleMouseMove
// Created: SBO 2008-07-07
// -----------------------------------------------------------------------------
bool TextLayer::HandleMouseMove( QMouseEvent*, const geometry::Point2f& point )
{
    for( T_Buttons::const_iterator it = buttons_.begin(); it != buttons_.end(); ++it )
        if( (*it)->HandleMouseMove( point ) )
            return true;
    return false;
}

// -----------------------------------------------------------------------------
// Name: TextLayer::Display
// Created: SBO 2008-07-07
// -----------------------------------------------------------------------------
void TextLayer::Display( const QString& text )
{
    ClearButtons();
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
    GlButton* button = new GlButton( label, tools_ );
    buttons_.push_back( button );
    button->SetPosition( Qt::AlignRight );
    button->SetMargin( 10, 10 );
}

// -----------------------------------------------------------------------------
// Name: TextLayer::ClearButtons
// Created: SBO 2009-02-11
// -----------------------------------------------------------------------------
void TextLayer::ClearButtons()
{
    for( T_Buttons::iterator it = buttons_.begin(); it != buttons_.end(); ++it )
        delete *it;
    buttons_.clear();
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
