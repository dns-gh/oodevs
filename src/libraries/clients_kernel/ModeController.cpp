// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#include "clients_kernel_pch.h"
#include "ModeController.h"
#include "ModesObserver_ABC.h"
#include "DisplayableModesObserver_ABC.h"
#include "OptionsController.h"
#include "Settings.h"
#include "Tools.h"
#pragma warning( push, 0 )
#pragma warning( disable : 4127 )
#include <QtCore/qsettings.h>
#include <QtGui/qmainwindow.h>
#pragma warning( pop )

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: ModeController constructor
// Created: ABR 2012-05-10
// -----------------------------------------------------------------------------
ModeController::ModeController()
    : parent_                ( 0 )
    , currentMode_           ( eModes_None )
    , useDefault_            ( true )
    , firstChangeToSavedMode_( true )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ModeController destructor
// Created: ABR 2012-05-10
// -----------------------------------------------------------------------------
ModeController::~ModeController()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ModeController::AddRegistryEntry
// Created: ABR 2013-02-15
// -----------------------------------------------------------------------------
void ModeController::AddRegistryEntry( E_Modes mode, const QString& registryEntry )
{
    if( registryModes_.find( mode ) != registryModes_.end() )
        throw MASA_EXCEPTION( QString( "The mode '%1' already have a registry entry: '%2'." )
                              .arg( ENT_Tr::ConvertFromModes( mode ).c_str() )
                              .arg( registryModes_[ mode ] ).toStdString() );
    registryModes_[ mode ] = registryEntry;
}

// -----------------------------------------------------------------------------
// Name: ModeController::ChangeMode
// Created: ABR 2012-05-10
// -----------------------------------------------------------------------------
void ModeController::ChangeMode( E_Modes newMode )
{
    if( newMode == currentMode_ )
        return;

    if( firstChangeToSavedMode_ && registryModes_.find( newMode ) != registryModes_.end() )
        LoadState( newMode );
    if( registryModes_.find( currentMode_ ) != registryModes_.end() )
        SaveState( currentMode_ );

    currentMode_ = newMode;

    // $$$$ ABR 2012-06-14: Update Displayable first so simple observer can update display when swapping mode (Save action for example)
    Apply( &DisplayableModesObserver_ABC::NotifyModeChanged, currentMode_, useDefault_, firstChangeToSavedMode_ );
    Apply( &ModesObserver_ABC::NotifyModeChanged, currentMode_ );

    if( firstChangeToSavedMode_ && registryModes_.find( newMode ) != registryModes_.end() )
        firstChangeToSavedMode_ = false;
}

// -----------------------------------------------------------------------------
// Name: ModeController::EnsureModeIsAvailableForRegistry
// Created: ABR 2013-02-15
// -----------------------------------------------------------------------------
void ModeController::EnsureModeIsAvailableForRegistry( E_Modes mode )
{
    if( registryModes_.find( mode ) == registryModes_.end() )
        throw MASA_EXCEPTION( QString( "The mode '%1' doesn't have any registry entry." ).arg( ENT_Tr::ConvertFromModes( mode ).c_str() ).toStdString() );
}

// -----------------------------------------------------------------------------
// Name: ModeController::LoadState
// Created: ABR 2012-05-10
// -----------------------------------------------------------------------------
void ModeController::LoadState( E_Modes mode )
{
    EnsureModeIsAvailableForRegistry( mode );
    QSettings settings( "MASA Group", "SWORD" );
    settings.beginGroup( "/" + registryModes_[ mode ] );
    if( !settings.contains( "mainWindowState" ) )
        return;
    assert( parent_ );
    parent_->restoreState( settings.value("mainWindowState").toByteArray() );
    useDefault_ = false;
}

// -----------------------------------------------------------------------------
// Name: ModeController::SaveState
// Created: ABR 2012-05-10
// -----------------------------------------------------------------------------
void ModeController::SaveState( E_Modes mode )
{
    EnsureModeIsAvailableForRegistry( mode );
    QSettings settings( "MASA Group", "SWORD" );
    settings.beginGroup( "/" + registryModes_[ mode ] );
    assert( parent_ );
    settings.setValue( "mainWindowState", parent_->saveState() );
    useDefault_ = false;
}

// -----------------------------------------------------------------------------
// Name: ModeController::LoadGeometry
// Created: ABR 2013-02-15
// -----------------------------------------------------------------------------
void ModeController::LoadGeometry( E_Modes mode )
{
    EnsureModeIsAvailableForRegistry( mode );
    QSettings settings( "MASA Group", "SWORD" );
    settings.beginGroup( "/" + registryModes_[ mode ] );
    assert( parent_ );
    parent_->restoreGeometry( settings.value( "mainWindowGeometry").toByteArray() );
}

// -----------------------------------------------------------------------------
// Name: ModeController::SaveGeometry
// Created: ABR 2013-02-15
// -----------------------------------------------------------------------------
void ModeController::SaveGeometry( E_Modes mode )
{
    EnsureModeIsAvailableForRegistry( mode );
    QSettings settings( "MASA Group", "SWORD" );
    settings.beginGroup( "/" + registryModes_[ mode ] );
    assert( parent_ );
    settings.setValue( "mainWindowGeometry", parent_->saveGeometry() );
}

// -----------------------------------------------------------------------------
// Name: ModeController::LoadOptions
// Created: ABR 2013-02-15
// -----------------------------------------------------------------------------
void ModeController::LoadOptions( E_Modes mode, OptionsController& options )
{
    EnsureModeIsAvailableForRegistry( mode );
    Settings settings( "MASA Group", "SWORD" );
    settings.beginGroup( "/" + registryModes_[ mode ] + "/Options" );
    options.Load( settings );
    settings.endGroup();
}

// -----------------------------------------------------------------------------
// Name: ModeController::SaveOptions
// Created: ABR 2013-02-15
// -----------------------------------------------------------------------------
void ModeController::SaveOptions( E_Modes mode, OptionsController& options )
{
    EnsureModeIsAvailableForRegistry( mode );
    Settings settings( "MASA Group", "SWORD" );
    settings.beginGroup( "/" + registryModes_[ mode ] + "/Options" );
    options.Save( settings );
    settings.endGroup();
}

// -----------------------------------------------------------------------------
// Name: ModeController::GetCurrentMode
// Created: ABR 2012-05-11
// -----------------------------------------------------------------------------
E_Modes ModeController::GetCurrentMode() const
{
    return currentMode_;
}

// -----------------------------------------------------------------------------
// Name: ModeController::SetMainWindow
// Created: LGY 2012-06-22
// -----------------------------------------------------------------------------
void ModeController::SetMainWindow( QMainWindow* parent )
{
    parent_ = parent;
}
