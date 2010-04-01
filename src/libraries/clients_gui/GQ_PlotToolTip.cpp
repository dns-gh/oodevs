// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2003 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "clients_gui_pch.h"
#include "GQ_PlotToolTip.h"

#include "GQ_Plot.h"

#include <qrect.h>
#include <qstringlist.h>

using namespace gui;

// -----------------------------------------------------------------------------
// Name: GQ_PlotToolTip constructor
/** @param  
*/
// Created: CBX 2003-08-18
// -----------------------------------------------------------------------------
GQ_PlotToolTip::GQ_PlotToolTip( GQ_Plot& plot )
: QToolTip( &plot, new QToolTipGroup( &plot ) )
, plot_   ( plot  )
{
    group()->setEnabled( true );
}

// -----------------------------------------------------------------------------
// Name: GQ_PlotToolTip destructor
// Created: CBX 2003-08-18
// -----------------------------------------------------------------------------
GQ_PlotToolTip::~GQ_PlotToolTip()
{
    // NOTHING
}


// -----------------------------------------------------------------------------
// Name: GQ_PlotToolTip::maybeTip
/** @param  QPoint& 
*/
// Created: CBX 2003-08-18
// -----------------------------------------------------------------------------
void GQ_PlotToolTip::maybeTip( const QPoint& point )
{
    QRect       tipRect;
    QStringList tipList;

    if( !plot_.GetToolTips( point, tipRect, tipList ) )
        return;

    // $$$$ CBX 2003-11-18: TODO: truncate list if it is too long or pan on several columns
    QString tipStr( tipList.join( "\n" ) );
    tip( tipRect, tipStr );
}
