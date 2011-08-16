// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "adaptation_app_pch.h"
#include "ADN_EditLine_ABC.h"
#include "moc_ADN_EditLine_ABC.cpp"
#include "ADN_Workspace.h"
#include "ADN_Enums.h"
#include "ADN_App.h"
#include "ADN_MainWindow.h"

//-----------------------------------------------------------------------------
// Name: ADN_EditLine_ABC constructor
// Created: JDY 03-07-09
//-----------------------------------------------------------------------------
ADN_EditLine_ABC::ADN_EditLine_ABC(QWidget* parent, const char * name)
    : QLineEdit(parent,name)
    , ADN_Gfx_ABC()
{
    // connect edit line & connector
    connect( this                   , SIGNAL( textChanged( const QString & ) ),
             this                   , SLOT( TextChanged( const QString & ) ) );
    connect( static_cast< ADN_App* >( qApp )->GetMainWindow(), SIGNAL(OpenModeToggled()), this, SLOT(UpdateEnableState()) );
}

//-----------------------------------------------------------------------------
// Name: ADN_EditLine_ABC destructor
// Created: JDY 03-07-09
//-----------------------------------------------------------------------------
ADN_EditLine_ABC::~ADN_EditLine_ABC()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ADN_EditLine_ABC::setEnabled
// Created: AGN 2004-05-25
// -----------------------------------------------------------------------------
void ADN_EditLine_ABC::setEnabled( bool b )
{
    if( bEnabledOnlyInAdminMode_ && b )
        QLineEdit::setEnabled( ADN_Workspace::GetWorkspace().GetOpenMode() == eOpenMode_Admin );
    else
        QLineEdit::setEnabled( b );
}

// -----------------------------------------------------------------------------
// Name: ADN_EditLine_ABC::focusOutEvent
// Created: APE 2005-04-13
// -----------------------------------------------------------------------------
void ADN_EditLine_ABC::focusOutEvent( QFocusEvent* pEvent )
{
    QLineEdit::focusOutEvent( pEvent );
}
