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
#include "MenuButton.h"
#include "clients_gui/Tools.h"

// -----------------------------------------------------------------------------
// Name: QuitPage constructor
// Created: SBO 2008-02-21
// -----------------------------------------------------------------------------
QuitPage::QuitPage( Q3WidgetStack* pages, Page_ABC& previous )
    : MenuPage( pages, *this, 0 )
{
    yesButton_ = AddLink( *this, SLOT( OnQuit() ) );
    noButton_ = AddLink( previous );
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
// Name: QuitPage::OnLanguageChanged
// Created: ABR 2011-11-09
// -----------------------------------------------------------------------------
void QuitPage::OnLanguageChanged()
{
    SetCenterTitle( tools::translate( "QuitPage", "Please confirm" ) );
    SetTextAndSubtitle( yesButton_, tools::translate( "QuitPage", "Yes" ) );
    SetTextAndSubtitle( noButton_, tools::translate( "QuitPage", "No" ) );
    MenuPage::OnLanguageChanged();
}

// -----------------------------------------------------------------------------
// Name: QuitPage::OnQuit
// Created: SBO 2008-02-21
// -----------------------------------------------------------------------------
void QuitPage::OnQuit()
{
    qApp->closeAllWindows();
}
