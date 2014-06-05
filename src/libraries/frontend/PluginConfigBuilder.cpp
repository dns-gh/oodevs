// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "frontend_pch.h"
#include "PluginConfigBuilder.h"
#include "CompositePluginConfig.h"
#include "PluginConfig.h"
#include "tools/DefaultLoader.h"
#include "tools/GeneralConfig.h"
#include "tools/NullFileLoaderObserver.h"
#include <boost/bind.hpp>
#include <xeumeuleu/xml.hpp>

using namespace frontend;

// -----------------------------------------------------------------------------
// Name: PluginConfigBuilder constructor
// Created: SBO 2011-05-09
// -----------------------------------------------------------------------------
PluginConfigBuilder::PluginConfigBuilder( const tools::GeneralConfig& config, QTabWidget* parent )
    : config_( config )
    , composite_( new CompositePluginConfig( parent ) )
    , observer_( new tools::NullFileLoaderObserver() )
    , loader_( new tools::DefaultLoader( *observer_ ) )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PluginConfigBuilder destructor
// Created: SBO 2011-05-09
// -----------------------------------------------------------------------------
PluginConfigBuilder::~PluginConfigBuilder()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PluginConfigBuilder::AddPlugin
// Created: ABR 2013-03-01
// -----------------------------------------------------------------------------
bool PluginConfigBuilder::AddPlugin( const tools::Path& path )
{
    if( path.FileName() == "plugin.xml" )
    {
        try
        {
            std::unique_ptr< xml::xistream > xis = loader_->LoadFile( path );
            *xis >> xml::start( "plugin" );
            composite_->Add( config_, *xis );
        }
        catch( const xml::exception& e )
        {
            observer_->NotifyInvalidXml( path, e );
        }
        catch( ... )
        {
            // NOTHING
        }
    }
    return false;
}

// -----------------------------------------------------------------------------
// Name: PluginConfigBuilder::BuildFromXml
// Created: SBO 2011-05-09
// -----------------------------------------------------------------------------
PluginConfigBuilder& PluginConfigBuilder::BuildFromXml()
{
    const tools::Path root = config_.BuildPluginDirectory( "" );
    if( composite_.get() && root.Exists() )
        root.Apply( boost::bind( &PluginConfigBuilder::AddPlugin, this, _1 ), true );
    return *this;
}

// -----------------------------------------------------------------------------
// Name: PluginConfigBuilder::Finalize
// Created: SBO 2011-05-10
// -----------------------------------------------------------------------------
PluginConfig_ABC* PluginConfigBuilder::Finalize()
{
    return composite_.release();
}
