// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2003 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: CBX 2003-11-25 $
// $Archive: /MVW_v10/Build/SDK/Adn2/src/GQ_PlotCaption.cpp $
// $Author: Ape $
// $Modtime: 13/01/05 12:16 $
// $Revision: 2 $
// $Workfile: GQ_PlotCaption.cpp $
//
// *****************************************************************************

#include "adaptation_app_pch.h"
#include "GQ_PlotCaption.h"
#include "moc_GQ_PlotCaption.cpp"

#include "GQ_Plot.h"

// -----------------------------------------------------------------------------
// Name: GQ_PlotCaption constructor
/** @param  pParent
*/
// Created: CBX 2003-11-25
// -----------------------------------------------------------------------------
GQ_PlotCaption::GQ_PlotCaption( QWidget* pParent, GQ_Plot& plot )
: Q3HBox       ( pParent )
, pPlot_        ( &plot   )
, pEntryList_   ( 0 )
, bUpdateNeeded_( true )
{
    connect( pPlot_, SIGNAL( DataTouched   () )
           , this,   SLOT  ( UpdateCaption () ) );

    pEntryList_ = new Q3ListBox( this );
    pEntryList_->setVariableWidth( true );
}

// -----------------------------------------------------------------------------
// Name: GQ_PlotCaption destructor
// Created: CBX 2003-11-25
// -----------------------------------------------------------------------------
GQ_PlotCaption::~GQ_PlotCaption()
{
    // NOTHING
}


// -----------------------------------------------------------------------------
// Name: GQ_PlotCaption::UpdateCaption
// Created: CBX 2003-11-25
// -----------------------------------------------------------------------------
void GQ_PlotCaption::UpdateCaption()
{
    if( bUpdateNeeded_ )
        return;

    bUpdateNeeded_ = true;
    update();
}

// -----------------------------------------------------------------------------
// Name: GQ_PlotCaption::paintEvent
// Created: CBX 2003-11-25
// -----------------------------------------------------------------------------
void GQ_PlotCaption::paintEvent( QPaintEvent* pEvent )
{
    if( bUpdateNeeded_ )
        Update();

    QWidget::paintEvent( pEvent );
}


// -----------------------------------------------------------------------------
// Name: GQ_PlotCaption::Update
// Created: CBX 2003-11-25
// -----------------------------------------------------------------------------
void GQ_PlotCaption::Update()
{
    pEntryList_->clear();

    GQ_Plot::iterator it  = pPlot_->begin();
    GQ_Plot::iterator end = pPlot_->end  ();
    for( uint i = 0; it != end; ++it, ++i )
    {
        GQ_PlotData& plotData = *it;

        if( plotData.IsEmpty() )
            continue;

        const QString dataName = plotData.Name();
        if( dataName.isEmpty() )
            continue;

        QPixmap dataCaption;
        plotData.DrawCaption( dataCaption );

        pEntryList_->insertItem( dataCaption, dataName );
    }

    bUpdateNeeded_ = false;
}
