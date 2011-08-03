// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

/* TRANSLATOR gui::DiffusionListLegendBar */

#include "clients_gui_pch.h"
#include "DiffusionListLegendBar.h"

using namespace gui;

// -----------------------------------------------------------------------------
// Name: DiffusionListLegendBar constructor
// Created: FPT 2011-06-21
// -----------------------------------------------------------------------------
DiffusionListLegendBar::DiffusionListLegendBar( QWidget* parent, const QColor& color, int size )
    : QWidget( parent )
    , color_( color )
    , size_( size )
{
    QHBoxLayout* layout = new QHBoxLayout();
    setLayout( layout );
}

// -----------------------------------------------------------------------------
// Name: DiffusionListLegendBar destructor
// Created: FPT 2011-06-21
// -----------------------------------------------------------------------------
DiffusionListLegendBar::~DiffusionListLegendBar()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: DiffusionListLegendBar paintEvent
// Created: FPT 2011-06-21
// -----------------------------------------------------------------------------
void DiffusionListLegendBar::paintEvent( QPaintEvent * )
{
    QPainter paint;
   if ( paint.begin( this ) )
    {
        paint.fillRect( 0, 0, size_, size_ / 2, color_ );
        paint.drawRect( 0, 0, size_, size_ / 2 );
        paint.end();
    }
}