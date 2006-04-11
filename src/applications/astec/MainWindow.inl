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
// $Archive: /MVW_v10/Build/SDK/Light2/src/MainWindow.inl $
// $Author: Age $
// $Modtime: 13/05/05 11:05 $
// $Revision: 7 $
// $Workfile: MainWindow.inl $
//
// *****************************************************************************

#include <qlabel.h>
#include "GL3DWidget.h"
#include "GLWidget.h"

// -----------------------------------------------------------------------------
// Name: MainWindow::GetMainWindow
// Created: APE 2004-05-12
// -----------------------------------------------------------------------------
inline
MainWindow& MainWindow::GetMainWindow()
{
    assert( pInstance_ != 0 );
    return *pInstance_;
}

// -----------------------------------------------------------------------------
// Name: MainWindow::GetQGLWidget
// Created: HME 2005-11-25
// -----------------------------------------------------------------------------
inline
QGLWidget* MainWindow::GetQGLWidget( bool b3Dmode) const
{
    if ( b3Dmode )
        return (QGLWidget*)pGL3DWidget_;
    else
        return (QGLWidget*)pGLWidget_;
}
