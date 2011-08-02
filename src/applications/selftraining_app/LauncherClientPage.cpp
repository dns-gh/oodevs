// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#include "selftraining_app_pch.h"
#include "LauncherClientPage.h"
#include "frontend/LauncherClient.h"
#include "clients_gui/Tools.h"

// -----------------------------------------------------------------------------
// Name: LauncherClientPage constructor
// Created: SBO 2010-10-25
// -----------------------------------------------------------------------------
LauncherClientPage::LauncherClientPage( Q3WidgetStack* pages, const QString& title, Page_ABC& previous, unsigned short buttonFlags, frontend::LauncherClient& launcher )
    : ContentPage( pages, title, previous, buttonFlags )
    , launcher_( launcher )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: LauncherClientPage destructor
// Created: SBO 2010-10-25
// -----------------------------------------------------------------------------
LauncherClientPage::~LauncherClientPage()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: LauncherClientPage::Connect
// Created: SBO 2010-10-25
// -----------------------------------------------------------------------------
void LauncherClientPage::Connect( const QString& host, unsigned short port )
{
    launcher_.Connect( host.ascii(), port, *this );
}

// -----------------------------------------------------------------------------
// Name: LauncherClientPage::OnConnectionSucceeded
// Created: SBO 2010-10-25
// -----------------------------------------------------------------------------
void LauncherClientPage::OnConnectionSucceeded()
{
    launcher_.QueryExerciseList();
}

// -----------------------------------------------------------------------------
// Name: LauncherClientPage::OnConnectionFailed
// Created: SBO 2010-10-25
// -----------------------------------------------------------------------------
void LauncherClientPage::OnConnectionFailed( const std::string& reason )
{
    OnError( tools::translate( "LauncherClientPage", "Failed to connect to host: %1." ).arg( reason.c_str() ).ascii() );
}

// -----------------------------------------------------------------------------
// Name: LauncherClientPage::OnConnectionLost
// Created: SBO 2010-10-25
// -----------------------------------------------------------------------------
void LauncherClientPage::OnConnectionLost( const std::string& /*reason*/ )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: LauncherClientPage::OnError
// Created: SBO 2010-10-25
// -----------------------------------------------------------------------------
void LauncherClientPage::OnError( const std::string& message )
{
    QMessageBox::critical( 0, tools::translate( "LauncherClientPage", "Error" ), message.c_str() );
}
