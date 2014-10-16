// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2014 MASA Group
//
// *****************************************************************************

#include "clients_kernel_pch.h"
#include "OptionsController.h"
#include "OptionsObserver_ABC.h"
#include "Options.h"
#include "Settings_ABC.h"
#include <boost/range/algorithm_ext/erase.hpp>

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: OptionsController constructor
// Created: ABR 2014-06-24
// -----------------------------------------------------------------------------
OptionsController::OptionsController()
    : generalOptions_( std::make_shared< Options >() )
    , viewOptions_( std::make_shared< Options >() )
{
    generalOptions_->InitializeGeneral();
    viewOptions_->InitializeView();
}

// -----------------------------------------------------------------------------
// Name: OptionsController destructor
// Created: ABR 2014-06-24
// -----------------------------------------------------------------------------
OptionsController::~OptionsController()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: OptionsController::InitializeLayers
// Created: ABR 2014-09-30
// -----------------------------------------------------------------------------
void OptionsController::InitializeLayers( const std::vector< E_LayerTypes >& layerOrder )
{
    viewOptions_->InitializeLayers( layerOrder );
}

// -----------------------------------------------------------------------------
// Name: OptionsController::Register
// Created: AGE 2006-02-13
// -----------------------------------------------------------------------------
void OptionsController::Register( tools::Observer_ABC& o )
{
    OptionsObserver_ABC* observer = dynamic_cast< OptionsObserver_ABC* >( &o );
    if( !observer )
        return;
    observers_.push_back( observer );
    generalOptions_->Apply( [&observer]( const std::string& name, const OptionVariant& option, bool ) {
        observer->OptionChanged( name, option );
    } );
    viewOptions_->Apply( [&observer]( const std::string& name, const OptionVariant& option, bool ) {
        observer->OptionChanged( name, option );
    } );
}

// -----------------------------------------------------------------------------
// Name: OptionsController::Unregister
// Created: AGE 2006-02-13
// -----------------------------------------------------------------------------
void OptionsController::Unregister( tools::Observer_ABC& o )
{
    OptionsObserver_ABC* observer = dynamic_cast< OptionsObserver_ABC* >( &o );
    boost::remove_erase( observers_, observer );
}

// -----------------------------------------------------------------------------
// Name: OptionsController::Change
// Created: AGE 2006-02-13
// -----------------------------------------------------------------------------
void OptionsController::Change( const std::string& name, const OptionVariant& value )
{
    if( generalOptions_->Has( name ) )
        generalOptions_->Set( name, value );
    else
        viewOptions_->Set( name, value );
    for( auto it = observers_.begin(); it != observers_.end(); ++it )
        ( *it )->OptionChanged( name, value );
}

// -----------------------------------------------------------------------------
// Name: OptionsController::GetOption
// Created: ABR 2014-06-24
// -----------------------------------------------------------------------------
const OptionVariant& OptionsController::GetOption( const std::string& name ) const
{
    return generalOptions_->Has( name ) ? generalOptions_->Get( name ) : viewOptions_->Get( name );
}

// -----------------------------------------------------------------------------
// Name: OptionsController::LoadGeneralOptions
// Created: ABR 2014-07-24
// -----------------------------------------------------------------------------
void OptionsController::LoadGeneralOptions( Settings_ABC& settings )
{
    generalOptions_->Load( settings );
    UpdateGeneralOptions();
}

// -----------------------------------------------------------------------------
// Name: OptionsController::SaveGeneralOptions
// Created: ABR 2014-07-24
// -----------------------------------------------------------------------------
void OptionsController::SaveGeneralOptions( Settings_ABC& settings )
{
    generalOptions_->Save( settings );
}

// -----------------------------------------------------------------------------
// Name: OptionsController::LoadViewOptions
// Created: ABR 2014-07-24
// -----------------------------------------------------------------------------
void OptionsController::LoadViewOptions( Settings_ABC& settings )
{
    viewOptions_->Load( settings );
    UpdateViewOptions();
}

// -----------------------------------------------------------------------------
// Name: OptionsController::SaveViewOptions
// Created: ABR 2014-07-24
// -----------------------------------------------------------------------------
void OptionsController::SaveViewOptions( Settings_ABC& settings )
{
    viewOptions_->Save( settings );
}

namespace
{
    void UpdateOptions( const std::vector< OptionsObserver_ABC* >& observers, Options& options )
    {
        options.Apply( [&observers]( const std::string& name, const OptionVariant& option, bool ) {
            std::for_each( observers.begin(), observers.end(), [&name,&option]( OptionsObserver_ABC* observer ) {
                observer->OptionChanged( name, option );
            } );
        } );
    }
}

// -----------------------------------------------------------------------------
// Name: OptionsController::UpdateGeneralOptions
// Created: ABR 2014-07-24
// -----------------------------------------------------------------------------
void OptionsController::UpdateGeneralOptions()
{
    UpdateOptions( observers_, *generalOptions_ );
}

// -----------------------------------------------------------------------------
// Name: OptionsController::UpdateAll
// Created: ABR 2014-07-17
// -----------------------------------------------------------------------------
void OptionsController::UpdateViewOptions()
{
    UpdateOptions( observers_, *viewOptions_ );
}

// -----------------------------------------------------------------------------
// Name: OptionsController::Remove
// Created: LGY 2010-09-24
// -----------------------------------------------------------------------------
void OptionsController::Remove( const std::string& name )
{
    if( generalOptions_->Has( name ) )
        generalOptions_->Remove( name );
    else 
        viewOptions_->Remove( name );
}

// -----------------------------------------------------------------------------
// Name: OptionsController::GetGeneralOptions
// Created: ABR 2014-07-24
// -----------------------------------------------------------------------------
const std::shared_ptr< Options >& OptionsController::GetGeneralOptions() const
{
    return generalOptions_;
}

// -----------------------------------------------------------------------------
// Name: OptionsController::GetViewOptions
// Created: ABR 2014-06-25
// -----------------------------------------------------------------------------
const std::shared_ptr< Options >& OptionsController::GetViewOptions() const
{
    return viewOptions_;
}

// -----------------------------------------------------------------------------
// Name: OptionsController::SetViewOptions
// Created: ABR 2014-07-18
// -----------------------------------------------------------------------------
void OptionsController::SetViewOptions( const std::shared_ptr< Options >& options )
{
    if( options )
        viewOptions_ = options;
}

// -----------------------------------------------------------------------------
// Name: OptionsController::SetGeneralOptions
// Created: ABR 2014-07-25
// -----------------------------------------------------------------------------
void OptionsController::SetGeneralOptions( const std::shared_ptr< Options >& options )
{
    if( options )
        generalOptions_ = options;
}

// -----------------------------------------------------------------------------
// Name: OptionsController::Load
// Created: JSR 2014-09-24
// -----------------------------------------------------------------------------
void OptionsController::Load( Settings_ABC& settings )
{
    settings.BeginGroup( "General" );
    LoadGeneralOptions( settings );
    settings.EndGroup();
    settings.BeginGroup( "MainView" );
    LoadViewOptions( settings );
    settings.EndGroup();
}

// -----------------------------------------------------------------------------
// Name: OptionsController::Save
// Created: JSR 2014-09-24
// -----------------------------------------------------------------------------
void OptionsController::Save( Settings_ABC& settings )
{
    // remove old configuration, can't use clear cause geometries are already written
    settings.RemoveKey( "/Options" );
    settings.RemoveKey( "/General" );
    settings.RemoveKey( "/MainView" );
    settings.BeginGroup( "General" );
    SaveGeneralOptions( settings );
    settings.EndGroup();
    settings.BeginGroup( "MainView" );
    SaveViewOptions( settings );
    settings.EndGroup();
}
