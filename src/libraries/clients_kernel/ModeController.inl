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
ModeController< EnumType >::ModeController( QMainWindow* parent, EnumType initialMode, const QString& registeryEntry, T_Converter converter )
    : parent_           ( parent )
    , currentMode_      ( initialMode )
    , registeryEntry_   ( registeryEntry )
    , converter_        ( converter)
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
// Name: ModeController::Initialize
// Created: ABR 2012-05-10
// -----------------------------------------------------------------------------
template< typename EnumType >
void ModeController< EnumType >::Initialize()
{
    bool useDefault = !LoadGeometry();
    Apply( &ModesObserver_ABC::NotifyModeChange, currentMode_, useDefault );
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
    SaveGeometry();
    currentMode_ = static_cast< EnumType >( newMode );
    bool useDefault = !LoadGeometry();
    Apply( &ModesObserver_ABC::NotifyModeChange, currentMode_, useDefault );
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
// Name: ModeController::LoadGeometry
// Created: ABR 2012-05-10
// -----------------------------------------------------------------------------
template< typename EnumType >
bool ModeController< EnumType >::LoadGeometry()
{
    const QString currentName = converter_( currentMode_, ENT_Tr_ABC::eToSim ).c_str();
    QSettings settings( "MASA Group", tools::translate( "Application", "SWORD" ) );
    settings.beginGroup( "/" + registeryEntry_ + "/States/" + currentName);
    if( !settings.contains( "mainWindowState" ) )
        return false;
    parent_->restoreState( settings.value("mainWindowState").toByteArray() );
    return true;
}

// -----------------------------------------------------------------------------
// Name: ModeController::SaveGeometry
// Created: ABR 2012-05-10
// -----------------------------------------------------------------------------
template< typename EnumType >
void ModeController< EnumType >::SaveGeometry()
{
    const QString currentName = converter_( currentMode_, ENT_Tr_ABC::eToSim ).c_str();
    QSettings settings( "MASA Group", tools::translate( "Application", "SWORD" ) );
    settings.beginGroup( "/" + registeryEntry_ + "/States/" + currentName);
    settings.setValue( "mainWindowState", parent_->saveState() );
}
