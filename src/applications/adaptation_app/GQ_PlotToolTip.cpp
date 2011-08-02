// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2003 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: CBX 2003-08-18 $
// $Archive: /MVW_v10/Build/SDK/Adn2/src/GQ_PlotToolTip.cpp $
// $Author: Ape $
// $Modtime: 17/12/04 11:48 $
// $Revision: 1 $
// $Workfile: GQ_PlotToolTip.cpp $
//
// *****************************************************************************

#ifdef __GNUG__
#   pragma implementation
#endif

#include "adaptation_app_pch.h"
#include "GQ_PlotToolTip.h"

#include "GQ_Plot.h"

#include <QtCore/qrect.h>
#include <QtCore/qstringlist.h>

// -----------------------------------------------------------------------------
// Name: GQ_PlotToolTip constructor
/** @param
*/
// Created: CBX 2003-08-18
// -----------------------------------------------------------------------------
GQ_PlotToolTip::GQ_PlotToolTip( GQ_Plot& plot )
: plot_( plot )
{
//    group()->setEnabled( true ); $$$$ FPT Old Qt3 tooltip api
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
    // GQ_LOG( "[ToolTip]: " << tipStr.ascii() );

    QToolTip::showText( point, tipList.join( "\n" ) );
}
