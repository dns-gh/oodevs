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
#include <qgroupbox.h>
#include <qtabbar.h>
#include <qtabwidget.h>

using namespace frontend;

namespace
{
    class TabWidget : public QTabWidget
    {
    public:
        explicit TabWidget( QWidget* parent ) : QTabWidget( parent )
        {
            setBackgroundOrigin( QWidget::WindowOrigin );
            tabBar()->setBackgroundOrigin( QWidget::WindowOrigin );
            setMargin( 0 );
        }
    };
}

// -----------------------------------------------------------------------------
// Name: CompositePluginConfig constructor
// Created: SBO 2011-05-09
// -----------------------------------------------------------------------------
CompositePluginConfig::CompositePluginConfig( QTabWidget* parent )
    : PluginConfig_ABC( parent )
{
    QGroupBox* pluginsBox = new QGroupBox( 1, Qt::Vertical, this );
    pluginsBox->setMargin( 5 );
    pluginsBox->setFrameShape( QFrame::NoFrame );
    pluginsBox->setBackgroundOrigin( QWidget::WindowOrigin );
    tabs_ = new TabWidget( pluginsBox );
    parent->addTab( this, tools::translate( "CompositePluginConfig", "Plugins" ) );
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
    PluginConfig_ABC* plugin = new PluginConfig( tabs_, config, xis );
    plugins_.push_back( plugin );
    tabs_->addTab( plugin, plugin->GetName() );
}
