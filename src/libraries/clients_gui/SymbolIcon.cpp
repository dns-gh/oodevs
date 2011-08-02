// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "clients_gui_pch.h"
#include "SymbolIcon.h"
#include <boost/tuple/tuple_comparison.hpp>

using namespace gui;

// -----------------------------------------------------------------------------
// Name: SymbolIcon constructor
// Created: AGE 2007-10-31
// -----------------------------------------------------------------------------
SymbolIcon::SymbolIcon( const std::string& symbol, const std::string& level /* = std::string()*/ )
    : symbol_( symbol )
    , level_ ( level )
    , color_ ( Qt::white )
    , size_  ( 32, 32 )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: SymbolIcon destructor
// Created: AGE 2007-10-31
// -----------------------------------------------------------------------------
SymbolIcon::~SymbolIcon()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: SymbolIcon::AddLevel
// Created: AGE 2007-10-31
// -----------------------------------------------------------------------------
void SymbolIcon::AddLevel( const std::string& level )
{
    level_ = level;
}

// -----------------------------------------------------------------------------
// Name: SymbolIcon::SetColor
// Created: AGE 2007-10-31
// -----------------------------------------------------------------------------
void SymbolIcon::SetColor( const QColor& color )
{
    color_ = color;
}

// -----------------------------------------------------------------------------
// Name: SymbolIcon::SetSize
// Created: AGE 2007-10-31
// -----------------------------------------------------------------------------
void SymbolIcon::SetSize( unsigned size )
{
    SetSize( size, size );
}

// -----------------------------------------------------------------------------
// Name: SymbolIcon::SetSize
// Created: AGE 2007-10-31
// -----------------------------------------------------------------------------
void SymbolIcon::SetSize( unsigned width, unsigned height )
{
    SetSize( QSize( width, height ) );
}

// -----------------------------------------------------------------------------
// Name: SymbolIcon::SetSize
// Created: AGE 2007-10-31
// -----------------------------------------------------------------------------
void SymbolIcon::SetSize( const QSize& size )
{
    size_ = size;
}

// -----------------------------------------------------------------------------
// Name: SymbolIcon::operator<
// Created: AGE 2007-10-31
// -----------------------------------------------------------------------------
bool SymbolIcon::operator<( const SymbolIcon& rhs ) const
{
    return boost::tuples::make_tuple(     symbol_,     level_,     color_.rgb(),     size_.width(),     size_.height() )
        <  boost::tuples::make_tuple( rhs.symbol_, rhs.level_, rhs.color_.rgb(), rhs.size_.width(), rhs.size_.height() );
}
