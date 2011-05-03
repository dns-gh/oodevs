// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "frontend_pch.h"
#include "VrForcesPluginConfigPanel.h"
#include "CreateSession.h"
#include "tools/GeneralConfig.h"
#include "clients_gui/Tools.h"
#include <qlineedit.h>
#include <qgroupbox.h>
#include <qlabel.h>
#include <boost/filesystem/convenience.hpp>

using namespace frontend;

namespace
{
    template< typename T >
    T* Style( T* widget )
    {
        widget->setBackgroundOrigin( QWidget::WindowOrigin );
        return widget;
    }
}

// -----------------------------------------------------------------------------
// Name: VrForcesPluginConfigPanel constructor
// Created: SBO 2011-04-18
// -----------------------------------------------------------------------------
VrForcesPluginConfigPanel::VrForcesPluginConfigPanel( QWidget* parent, const tools::GeneralConfig& config )
    : PluginConfig_ABC( parent )
    , config_( config )
    , library_( "vrforces_plugin-vc80-mt.dll" ) // $$$$ SBO 2011-04-18: hard coded library name
{
    setMargin( 5 );
    setBackgroundOrigin( QWidget::WindowOrigin );
    box_ = Style( new QGroupBox( 2, Horizontal, tools::translate( "VrForcesPluginConfigPanel", "Enable VrForces integration" ), this ) );
    box_->setCheckable( true );
    box_->setChecked( false );
    {
        Style( new QLabel( tools::translate( "VrForcesPluginConfigPanel", "FOM: " ), box_ ) );
        QHBox* box = Style( new QHBox( box_ ) );
        fom_ = Style( new QLineEdit( "VR-Link.fed", box ) );
    }
    {
        Style( new QLabel( tools::translate( "VrForcesPluginConfigPanel", "Models: " ), box_ ) );
        QHBox* box = Style( new QHBox( box_ ) );
        models_ = Style( new QLineEdit( "../data/simulationModelSets/default.sms", box ) );
    }
    {
        Style( new QLabel( tools::translate( "VrForcesPluginConfigPanel", "Scenario: " ), box_ ) );
        QHBox* box = Style( new QHBox( box_ ) );
        scenario_ = Style( new QLineEdit( "../data/terrain/WorldFlatEarth.mtd", box ) );
    }
}

// -----------------------------------------------------------------------------
// Name: VrForcesPluginConfigPanel destructor
// Created: SBO 2011-04-18
// -----------------------------------------------------------------------------
VrForcesPluginConfigPanel::~VrForcesPluginConfigPanel()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: VrForcesPluginConfigPanel::IsAvailable
// Created: SBO 2011-04-18
// -----------------------------------------------------------------------------
bool VrForcesPluginConfigPanel::IsAvailable() const
{
    return boost::filesystem::exists( library_ );
}

// -----------------------------------------------------------------------------
// Name: VrForcesPluginConfigPanel::Commit
// Created: SBO 2011-04-18
// -----------------------------------------------------------------------------
void VrForcesPluginConfigPanel::Commit( const std::string& exercise, const std::string& session )
{
    if( box_->isChecked() )
    {
        frontend::CreateSession action( config_, exercise, session );
        action.SetOption( "session/config/dispatcher/plugins/vrforces/@library", library_ );
        action.SetOption( "session/config/dispatcher/plugins/vrforces/@fom", fom_->text() );
        action.SetOption( "session/config/dispatcher/plugins/vrforces/@models", models_->text() );
        action.SetOption( "session/config/dispatcher/plugins/vrforces/@scenario", scenario_->text() );
        action.Commit();
    }
}
