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
// $Archive: /MVW_v10/Build/SDK/MOS_Light2/src/MOS_MapEventFilter_ABC.cpp $
// $Author: Age $
// $Modtime: 4/07/05 11:12 $
// $Revision: 5 $
// $Workfile: MOS_MapEventFilter_ABC.cpp $
//
// *****************************************************************************

#ifdef __GNUG__
#   pragma implementation
#endif

#include "MOS_Light2_pch.h"
#include "MOS_MapEventFilter_ABC.h"

#include "MOS_MainWindow.h"


// -----------------------------------------------------------------------------
// Name: MOS_MapEventFilter_ABC constructor
// Created: APE 2004-09-01
// -----------------------------------------------------------------------------
MOS_MapEventFilter_ABC::MOS_MapEventFilter_ABC( bool bBlocking )
    : bPushed_  ( false )
    , bBlocking_( bBlocking )
{
}


// -----------------------------------------------------------------------------
// Name: MOS_MapEventFilter_ABC destructor
// Created: APE 2004-09-01
// -----------------------------------------------------------------------------
MOS_MapEventFilter_ABC::~MOS_MapEventFilter_ABC()
{
    // NOTHING
}


// -----------------------------------------------------------------------------
// Name: MOS_MapEventFilter_ABC::Push
// Created: APE 2004-09-01
// -----------------------------------------------------------------------------
void MOS_MapEventFilter_ABC::Push()
{
    if( bPushed_ == false )
    {
        MOS_MainWindow::GetMainWindow().PushMapEventFilter( *this );
        bPushed_ = true;
    }
}


// -----------------------------------------------------------------------------
// Name: MOS_MapEventFilter_ABC::Pop
// Created: APE 2004-09-01
// -----------------------------------------------------------------------------
void MOS_MapEventFilter_ABC::Pop()
{
    if( bPushed_ == true )
    {
        // $$$$ AGE 2005-07-04: n'importe quoi. Singletons are evil !
        if( & MOS_MainWindow::GetMainWindow() )
            MOS_MainWindow::GetMainWindow().PopMapEventFilter( *this );
        bPushed_ = false;
    }
}


// -----------------------------------------------------------------------------
// Name: MOS_MapEventFilter_ABC::Draw
// Created: APE 2004-09-01
// -----------------------------------------------------------------------------
void MOS_MapEventFilter_ABC::Draw() const
{
}
