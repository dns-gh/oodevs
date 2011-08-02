// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "selftraining_app_pch.h"
#include "ContentPage.h"

// -----------------------------------------------------------------------------
// Name: ContentPage constructor
// Created: SBO 2008-02-21
// -----------------------------------------------------------------------------
ContentPage::ContentPage( Q3WidgetStack* pages, const QString& title, Page_ABC& previous, unsigned short buttonFlags )
    : Page_ABC( pages, previous, buttonFlags )
{
    AddTitle( title );
}

// -----------------------------------------------------------------------------
// Name: ContentPage destructor
// Created: SBO 2008-02-21
// -----------------------------------------------------------------------------
ContentPage::~ContentPage()
{
    // NOTHING
}

