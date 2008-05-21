// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "DispatcherFacade.h"
#include "dispatcher/Config.h"
#include "dispatcher/Dispatcher.h"
#include "hla_plugin/HlaPluginFactory.h"
#include "dis_plugin/DisPluginFactory.h"
#include "tic_plugin/TicPluginFactory.h"
#include "bml_plugin/BmlPluginFactory.h"
#ifdef CROSSBOW_PLUGIN
#   include "gearth_plugin/GearthPluginFactory.h"
#   include "crossbow_plugin/CrossbowPluginFactory.h"
#endif

// -----------------------------------------------------------------------------
// Name: DispatcherFacade constructor
// Created: AGE 2008-05-21
// -----------------------------------------------------------------------------
DispatcherFacade::DispatcherFacade( int argc, char** argv )
    : config_  ( new dispatcher::Config() )
{
    config_->Parse( argc, argv );
    dispatcher_.reset( new dispatcher::Dispatcher( *config_ ) );
    dispatcher_->RegisterPluginFactory( *new hla::HlaPluginFactory() );
    dispatcher_->RegisterPluginFactory( *new dis::DisPluginFactory() );
    dispatcher_->RegisterPluginFactory( *new tic::TicPluginFactory() );
    dispatcher_->RegisterPluginFactory( *new bml::BmlPluginFactory() );
#ifdef CROSSBOW_PLUGIN
    dispatcher_->RegisterPluginFactory( *new crossbow::CrossbowPluginFactory() );
    dispatcher_->RegisterPluginFactory( *new gearth::GearthPluginFactory() );
#endif
    dispatcher_->CreatePlugins();
}

// -----------------------------------------------------------------------------
// Name: DispatcherFacade destructor
// Created: AGE 2008-05-21
// -----------------------------------------------------------------------------
DispatcherFacade::~DispatcherFacade()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: DispatcherFacade::Update
// Created: AGE 2008-05-21
// -----------------------------------------------------------------------------
void DispatcherFacade::Update()
{
    dispatcher_->Update();
}
