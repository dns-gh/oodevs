// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#include "selftraining_app_pch.h"
#include "QuitPage.h"
#include "moc_QuitPage.cpp"
#include "clients_gui/Tools.h"
#include <qapplication.h>

// -----------------------------------------------------------------------------
// Name: QuitPage constructor
// Created: SBO 2008-02-21
// -----------------------------------------------------------------------------
QuitPage::QuitPage( QWidgetStack* pages, Page_ABC& previous )
    : MenuPage( pages, tools::translate( "QuitPage", "Please confirm" )  )
{
    AddLink( tools::translate( "QuitPage", "Yes" ), *this, true, " ", SLOT( OnQuit() ) );
    AddLink( tools::translate( "QuitPage", "No" ) , previous );
}

// -----------------------------------------------------------------------------
// Name: QuitPage destructor
// Created: SBO 2008-02-21
// -----------------------------------------------------------------------------
QuitPage::~QuitPage()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: QuitPage::OnQuit
// Created: SBO 2008-02-21
// -----------------------------------------------------------------------------
void QuitPage::OnQuit()
{
    qApp->closeAllWindows();
}
