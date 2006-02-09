// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: APE 2004-09-01 $
// $Archive: /MVW_v10/Build/SDK/Light2/src/MapEventFilter_ABC.cpp $
// $Author: Age $
// $Modtime: 4/07/05 11:12 $
// $Revision: 5 $
// $Workfile: MapEventFilter_ABC.cpp $
//
// *****************************************************************************

#ifdef __GNUG__
#   pragma implementation
#endif

#include "astec_pch.h"
#include "MapEventFilter_ABC.h"

#include "MainWindow.h"


// -----------------------------------------------------------------------------
// Name: MapEventFilter_ABC constructor
// Created: APE 2004-09-01
// -----------------------------------------------------------------------------
MapEventFilter_ABC::MapEventFilter_ABC( bool bBlocking )
    : bPushed_  ( false )
    , bBlocking_( bBlocking )
{
}


// -----------------------------------------------------------------------------
// Name: MapEventFilter_ABC destructor
// Created: APE 2004-09-01
// -----------------------------------------------------------------------------
MapEventFilter_ABC::~MapEventFilter_ABC()
{
    // NOTHING
}


// -----------------------------------------------------------------------------
// Name: MapEventFilter_ABC::Push
// Created: APE 2004-09-01
// -----------------------------------------------------------------------------
void MapEventFilter_ABC::Push()
{
    if( bPushed_ == false )
    {
        MainWindow::GetMainWindow().PushMapEventFilter( *this );
        bPushed_ = true;
    }
}


// -----------------------------------------------------------------------------
// Name: MapEventFilter_ABC::Pop
// Created: APE 2004-09-01
// -----------------------------------------------------------------------------
void MapEventFilter_ABC::Pop()
{
    if( bPushed_ == true )
    {
        // $$$$ AGE 2005-07-04: n'importe quoi. Singletons are evil !
        if( & MainWindow::GetMainWindow() )
            MainWindow::GetMainWindow().PopMapEventFilter( *this );
        bPushed_ = false;
    }
}


// -----------------------------------------------------------------------------
// Name: MapEventFilter_ABC::Draw
// Created: APE 2004-09-01
// -----------------------------------------------------------------------------
void MapEventFilter_ABC::Draw() const
{
}
