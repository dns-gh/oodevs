// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

// -----------------------------------------------------------------------------
// Name: ModeController constructor
// Created: ABR 2012-05-10
// -----------------------------------------------------------------------------
template< typename EnumType >
ModeController< EnumType >::ModeController( EnumType savedMode, const QString& registeryEntry )
    : parent_                ( 0 )
    , currentMode_           ( static_cast< EnumType >( 0 ) )
    , savedMode_             ( savedMode )
    , registeryEntry_        ( registeryEntry )
    , useDefault_            ( true )
    , firstChangeToSavedMode_( true )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ModeController destructor
// Created: ABR 2012-05-10
// -----------------------------------------------------------------------------
template< typename EnumType >
ModeController< EnumType >::~ModeController()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ModeController::ChangeMode
// Created: ABR 2012-05-10
// -----------------------------------------------------------------------------
template< typename EnumType >
void ModeController< EnumType >::ChangeMode( int newMode )
{
    if( static_cast< EnumType >( newMode ) == currentMode_ )
        return;
    if( firstChangeToSavedMode_ && static_cast< EnumType >( newMode ) == savedMode_ )
        LoadGeometry();
    if( currentMode_ == savedMode_ )
        SaveGeometry();

    currentMode_ = static_cast< EnumType >( newMode );
    // $$$$ ABR 2012-06-14: Update Displayable first so simple observer can update display when swapping mode (Save action for example)
    Apply( &DisplayableModesObserver_ABC::NotifyModeChanged, currentMode_, useDefault_, firstChangeToSavedMode_ );
    Apply( &ModesObserver_ABC::NotifyModeChanged, currentMode_ );

    if( firstChangeToSavedMode_ && static_cast< EnumType >( newMode ) == savedMode_ )
        firstChangeToSavedMode_ = false;
}

// -----------------------------------------------------------------------------
// Name: ModeController::LoadGeometry
// Created: ABR 2012-05-10
// -----------------------------------------------------------------------------
template< typename EnumType >
void ModeController< EnumType >::LoadGeometry()
{
    QSettings settings( "MASA Group", "SWORD" );
    settings.beginGroup( "/" + registeryEntry_ );
    if( !settings.contains( "mainWindowState" ) )
        return;
    assert( parent_ );
    parent_->restoreState( settings.value("mainWindowState").toByteArray() );
    useDefault_ = false;
}

// -----------------------------------------------------------------------------
// Name: ModeController::SaveGeometry
// Created: ABR 2012-05-10
// -----------------------------------------------------------------------------
template< typename EnumType >
void ModeController< EnumType >::SaveGeometry()
{
    QSettings settings( "MASA Group", "SWORD" );
    settings.beginGroup( "/" + registeryEntry_ );
    assert( parent_ );
    settings.setValue( "mainWindowState", parent_->saveState() );
    useDefault_ = false;
}

// -----------------------------------------------------------------------------
// Name: ModeController::GetCurrentMode
// Created: ABR 2012-05-11
// -----------------------------------------------------------------------------
template< typename EnumType >
int ModeController< EnumType >::GetCurrentMode() const
{
    return currentMode_;
}

// -----------------------------------------------------------------------------
// Name: ModeController::GetRegisteryEntry
// Created: ABR 2012-05-16
// -----------------------------------------------------------------------------
template< typename EnumType >
const QString& ModeController< EnumType >::GetRegisteryEntry() const
{
    return registeryEntry_;
}

// -----------------------------------------------------------------------------
// Name: ModeController::SetMainWindow
// Created: LGY 2012-06-22
// -----------------------------------------------------------------------------
template< typename EnumType >
void ModeController< EnumType >::SetMainWindow( QMainWindow* parent )
{
    parent_ = parent;
}
