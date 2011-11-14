// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "frontend_pch.h"
#include "CompositePluginConfig.h"
#include "PluginConfig.h"
#include <clients_kernel/Tools.h>
#include <algorithm>
#include <boost/bind.hpp>
#include <Qt3Support/q3groupbox.h>

using namespace frontend;

// -----------------------------------------------------------------------------
// Name: CompositePluginConfig constructor
// Created: SBO 2011-05-09
// -----------------------------------------------------------------------------
CompositePluginConfig::CompositePluginConfig( QTabWidget* parent )
    : PluginConfig_ABC( parent )
    , parent_( parent )
{
    Q3GroupBox* pluginsBox = new Q3GroupBox( 1, Qt::Vertical, this );
    pluginsBox->setMargin( 5 );
    pluginsBox->setFrameShape( Q3GroupBox::NoFrame );
    tabs_ = new QTabWidget( pluginsBox );
    parentTabIndex_ = parent->addTab( this, "" );
}

// -----------------------------------------------------------------------------
// Name: CompositePluginConfig destructor
// Created: SBO 2011-05-09
// -----------------------------------------------------------------------------
CompositePluginConfig::~CompositePluginConfig()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: CompositePluginConfig::OnLanguageChanged
// Created: ABR 2011-11-10
// -----------------------------------------------------------------------------
void CompositePluginConfig::OnLanguageChanged()
{
    parent_->setTabText( parentTabIndex_, tools::translate( "CompositePluginConfig", "Plugins" ) );
    for( int i = 0; i < tabs_->count(); ++i )
    {
        plugins_[ i ]->OnLanguageChanged();
        tabs_->setTabText( i, plugins_[ i ]->GetName() );
    }
}

// -----------------------------------------------------------------------------
// Name: CompositePluginConfig::GetName
// Created: SBO 2011-05-09
// -----------------------------------------------------------------------------
QString CompositePluginConfig::GetName() const
{
    return "composite";
}

// -----------------------------------------------------------------------------
// Name: CompositePluginConfig::IsAvailable
// Created: SBO 2011-05-09
// -----------------------------------------------------------------------------
bool CompositePluginConfig::IsAvailable() const
{
    for( std::vector< PluginConfig_ABC* >::const_iterator it = plugins_.begin(); it != plugins_.end(); ++it )
        if( !(*it)->IsAvailable() )
            return false;
    return true;
}

// -----------------------------------------------------------------------------
// Name: CompositePluginConfig::Commit
// Created: SBO 2011-05-09
// -----------------------------------------------------------------------------
void CompositePluginConfig::Commit( const std::string& exercise, const std::string& session )
{
    std::for_each( plugins_.begin(), plugins_.end(), boost::bind( &PluginConfig_ABC::Commit, _1, exercise, session ) );
}

// -----------------------------------------------------------------------------
// Name: CompositePluginConfig::Add
// Created: SBO 2011-05-09
// -----------------------------------------------------------------------------
void CompositePluginConfig::Add( const tools::GeneralConfig& config, xml::xistream& xis )
{
    std::auto_ptr< PluginConfig_ABC > plugin( new PluginConfig( tabs_, config, xis ) );
    if( plugin->IsAvailable() )
    {
        plugins_.push_back( plugin.release() );
        tabs_->addTab( plugins_.back(), "" );
    }
}
