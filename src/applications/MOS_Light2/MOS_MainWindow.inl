// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: APE 2004-03-01 $
// $Archive: /MVW_v10/Build/SDK/MOS_Light2/src/MOS_MainWindow.inl $
// $Author: Age $
// $Modtime: 13/05/05 11:05 $
// $Revision: 7 $
// $Workfile: MOS_MainWindow.inl $
//
// *****************************************************************************

#include <qlabel.h>


// -----------------------------------------------------------------------------
// Name: MOS_MainWindow::GetMainWindow
/** @return 
*/
// Created: APE 2004-05-12
// -----------------------------------------------------------------------------
MOS_INLINE
MOS_MainWindow& MOS_MainWindow::GetMainWindow()
{
    assert( pInstance_ != 0 );
    return *pInstance_;
}

// -----------------------------------------------------------------------------
// Name: MOS_MainWindow::GetOptions
/** @return 
*/
// Created: APE 2004-06-03
// -----------------------------------------------------------------------------
MOS_INLINE
MOS_Options& MOS_MainWindow::GetOptions() const
{
    return *pOptions_;
}
