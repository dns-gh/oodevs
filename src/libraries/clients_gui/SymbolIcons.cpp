// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 MASA Group
//
// *****************************************************************************

#include "clients_gui_pch.h"
#include "SymbolIcons.h"
#include "GL2DWidget.h"
#include "SignalAdapter.h"

using namespace gui;

SymbolIcons::SymbolIcons()
    : defaultSymbol_( 1, 1 )
    , context_( 0 )
    , widget_( 0 )
{
    // NOTHING
}

SymbolIcons::~SymbolIcons()
{
    // NOTHING
}

namespace
{
    const int iconSize = 256;
    const float viewSize = 600;
}

void SymbolIcons::Initialize( GL2DWidget* widget )
{
    if( !widget )
        throw MASA_EXCEPTION( "cannot initialize symbols icons with a null 2D widget" );
    if( widget == widget_ )
        return;
    context_ = new QGLWidget( 0, widget );
    context_->resize( iconSize, iconSize );
    context_->makeCurrent();
    glClearColor( 1, 1, 1, 0 );
    glViewport( 0, 0, iconSize, iconSize );
    glMatrixMode( GL_PROJECTION );
    glLoadIdentity();
    glMatrixMode( GL_MODELVIEW );
    glLoadIdentity();
    glOrtho( 0, viewSize, 0, viewSize, -300, 300 );
    glEnableClientState( GL_VERTEX_ARRAY );
    widget_ = widget;
    gui::connect( widget, SIGNAL( destroyed( QObject* ) ),
        [&]()
        {
            delete context_;
            widget_ = 0;
            context_ = 0;
        } );
    icons_.clear();
}

void SymbolIcons::Draw( std::string symbol, const geometry::Point2f& center, float factor ) const
{
    if( symbol.empty() )
        return;
    std::replace( symbol.begin(), symbol.end(), '*', 'f' );
    widget_->DrawApp6SymbolFixedSize( symbol, center, factor );
}

QPixmap SymbolIcons::GenerateSymbol( const SymbolIcon& symbol )
{
    context_->makeCurrent();
    glClear( GL_COLOR_BUFFER_BIT );
    widget_->SetCurrentColor( symbol.color_.red() / 255.f, symbol.color_.green() / 255.f, symbol.color_.blue() / 255.f );
    if( symbol.symbol_.find( "infrastructures" ) == std::string::npos )
    {
        const geometry::Point2f center( 300, 100 );
        Draw( symbol.symbol_, center, 1.f );
        Draw( symbol.level_, center, 1.f );
    }
    else
    {
        const geometry::Point2f center( 220, 310 );
        Draw( symbol.symbol_, center, 0.5f );
    }
    QImage image( iconSize, iconSize, QImage::Format_ARGB32 );
    glFinish();
    glReadPixels( 0, 0, iconSize, iconSize, GL_BGRA_EXT, GL_UNSIGNED_BYTE, image.bits() );
    QPixmap result( image.mirror().smoothScale( symbol.size_ ) );
    return result;
}

const QPixmap& SymbolIcons::GetSymbol( const SymbolIcon& symbol )
{
    if( !context_ || !widget_ )
        return GetDefaultSymbol();
    QPixmap& result = icons_[ symbol ];
    if( result.isNull() )
        result = GenerateSymbol( symbol );
    return result;
}

const QPixmap& SymbolIcons::GetDefaultSymbol() const
{
    return defaultSymbol_;
}
