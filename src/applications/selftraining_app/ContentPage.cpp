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
ContentPage::ContentPage( Q3WidgetStack* pages, Page_ABC& previous, unsigned short buttonFlags /*= 0*/, const QString title /*= QString()*/ )
    : Page_ABC( pages, previous, buttonFlags )
{
    if( !title.isEmpty() )
        SetTitle( title );
}

// -----------------------------------------------------------------------------
// Name: ContentPage destructor
// Created: SBO 2008-02-21
// -----------------------------------------------------------------------------
ContentPage::~ContentPage()
{
    // NOTHING
}

