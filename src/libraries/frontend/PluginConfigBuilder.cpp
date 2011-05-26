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
#include <boost/filesystem.hpp>
#include <xeumeuleu/xml.hpp>

namespace bfs = boost::filesystem;

using namespace frontend;

// -----------------------------------------------------------------------------
// Name: PluginConfigBuilder constructor
// Created: SBO 2011-05-09
// -----------------------------------------------------------------------------
PluginConfigBuilder::PluginConfigBuilder( const tools::GeneralConfig& config, QTabWidget* parent )
    : config_   ( config )
    , composite_( new CompositePluginConfig( parent ) )
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
// Name: PluginConfigBuilder::BuildFromXml
// Created: SBO 2011-05-09
// -----------------------------------------------------------------------------
PluginConfigBuilder& PluginConfigBuilder::BuildFromXml()
{
    const bfs::path root( config_.BuildPluginDirectory( "" ) );
    if( composite_.get() && bfs::exists( root ) )
    {
        tools::NullFileLoaderObserver observer;
        tools::DefaultLoader loader( observer );
        bfs::recursive_directory_iterator end;
        for( bfs::recursive_directory_iterator it( root ); it != end; ++it )
            if( it->path().leaf() == "plugin.xml" )
            {
                try
                {
                    std::auto_ptr< xml::xistream > xis = loader.LoadFile( it->path().string() );
                    *xis >> xml::start( "plugin" );
                    composite_->Add( config_, *xis );
                }
                catch( ... )
                {
                    // NOTHING
                }
                it.no_push();
            }
    }
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
