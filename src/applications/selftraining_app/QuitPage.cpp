// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "selftraining_app_pch.h"
#include "QuitPage.h"
#include "moc_QuitPage.cpp"
#include "clients_gui/Tools.h"

// -----------------------------------------------------------------------------
// Name: QuitPage constructor
// Created: SBO 2008-02-21
// -----------------------------------------------------------------------------
QuitPage::QuitPage( Q3WidgetStack* pages, Page_ABC& previous )
    : MenuPage( pages, *this, 0, tools::translate( "QuitPage", "Please confirm" )  )
{
    AddLink( tools::translate( "QuitPage", "Yes" ), *this, " ", SLOT( OnQuit() ) );
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
