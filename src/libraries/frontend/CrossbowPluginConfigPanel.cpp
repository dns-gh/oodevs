// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "frontend_pch.h"
#include "CrossbowPluginConfigPanel.h"
#include "CreateSession.h"
#include "tools/GeneralConfig.h"
#include "clients_gui/Tools.h"
#include <qlineedit.h>
#include <qspinbox.h>
#include <qgroupbox.h>
#include <qcheckbox.h>
#include <qlabel.h>
#include <qtooltip.h>

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
// Name: CrossbowPluginConfigPanel constructor
// Created: SBO 2008-03-05
// -----------------------------------------------------------------------------
CrossbowPluginConfigPanel::CrossbowPluginConfigPanel( QWidget* parent, const tools::GeneralConfig& config )
    : PluginConfig_ABC( parent )
    , config_( config )
{
    box_ = Style( new QGroupBox( 2, Horizontal, tools::translate( "CrossbowPluginConfigPanel", "Enable Crossbow export" ), this ) );
    box_->setCheckable( true );
    box_->setChecked( false );
    {
        Style( new QLabel( tools::translate( "CrossbowPluginConfigPanel", "Geodatabase export:" ), box_ ) );
        geodatabase_ = Style( new QLineEdit( "sde://user:password@host:port/sword_crossbow_db.sword", box_ ) );
        QToolTip::add( geodatabase_, tools::translate( "CrossbowPluginConfigPanel", "GDB: filename.{gdb|mdb}\nSDE connection: sde://user:password@host:port/db_name.sword" ) );
    }
    {
        Style( new QLabel( tools::translate( "CrossbowPluginConfigPanel", "Geodatabase population export:" ), box_ ) );
        population_ = Style( new QLineEdit( box_ ) );
    }
    {
        Style( new QLabel( tools::translate( "CrossbowPluginConfigPanel", "Geodatabase order import:" ), box_ ) );
        shared_ = Style( new QLineEdit( box_ ) );
    }
}

// -----------------------------------------------------------------------------
// Name: CrossbowPluginConfigPanel destructor
// Created: SBO 2008-03-05
// -----------------------------------------------------------------------------
CrossbowPluginConfigPanel::~CrossbowPluginConfigPanel()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: CrossbowPluginConfigPanel::Commit
// Created: SBO 2008-03-05
// -----------------------------------------------------------------------------
void CrossbowPluginConfigPanel::Commit( const std::string& exercise, const std::string& session )
{
    if( box_->isChecked() )
    {
        frontend::CreateSession action( config_, exercise, session );
        action.SetOption( "session/config/simulation/dispatcher/@embedded", false );
        action.SetOption( "session/config/simulation/dispatcher/@path", "./dispatcher/" );
        action.SetOption( "session/config/dispatcher/plugins/crossbow/@geodatabase", geodatabase_->text() );
        action.SetOption( "session/config/dispatcher/plugins/crossbow/@geodatabase-population", population_->text() );
        action.SetOption( "session/config/dispatcher/plugins/crossbow/@geodatabase-shared", shared_->text() );
    }
}
