// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "adaptation_app_pch.h"
#include "ADN_TextEdit_ABC.h"
#include "moc_ADN_TextEdit_ABC.cpp"
#include "ADN_App.h"
#include "ADN_Enums.h"
#include "ADN_MainWindow.h"

//-----------------------------------------------------------------------------
// Name: ADN_TextEdit_ABC constructor
// Created: JSR 21-11-11
//-----------------------------------------------------------------------------
ADN_TextEdit_ABC::ADN_TextEdit_ABC( QWidget* parent, const char * name)
    : QTextEdit( parent,name )
    , ADN_Gfx_ABC()
{
    // connect edit line & connector
    connect( this, SIGNAL( textChanged() ), this, SLOT( TextChanged() ) );
    connect( static_cast< ADN_App* >( qApp )->GetMainWindow(), SIGNAL( OpenModeToggled() ), this, SLOT( UpdateEnableState() ) );
}

//-----------------------------------------------------------------------------
// Name: ADN_TextEdit_ABC destructor
// Created: JSR 21-11-11
//-----------------------------------------------------------------------------
ADN_TextEdit_ABC::~ADN_TextEdit_ABC()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ADN_TextEdit_ABC::setEnabled
// Created: JSR 21-11-11
// -----------------------------------------------------------------------------
void ADN_TextEdit_ABC::setEnabled( bool b )
{
    if( bEnabledOnlyInAdminMode_ && b )
        QTextEdit::setEnabled( ADN_Workspace::GetWorkspace().GetOpenMode() == eOpenMode_Admin );
    else
        QTextEdit::setEnabled( b );
}
