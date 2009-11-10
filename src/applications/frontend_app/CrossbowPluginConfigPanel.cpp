// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "frontend_app_pch.h"
#include "CrossbowPluginConfigPanel.h"
#include "tools/GeneralConfig.h"
#include "frontend/CreateSession.h"
#include "clients_gui/Tools.h"
#include <qlineedit.h>
#include <qspinbox.h>
#include <qgroupbox.h>
#include <qcheckbox.h>
#include <qlabel.h>

// -----------------------------------------------------------------------------
// Name: CrossbowPluginConfigPanel constructor
// Created: SBO 2008-03-05
// -----------------------------------------------------------------------------
CrossbowPluginConfigPanel::CrossbowPluginConfigPanel( QWidget* parent, const tools::GeneralConfig& config )
    : QHBox( parent )
    , config_( config )
{
    box_ = new QGroupBox( 2, Horizontal, tools::translate( "CrossbowPluginConfigPanel", "Enable Crossbow export" ), this );
    box_->setCheckable( true );
    box_->setChecked( false );
    new QLabel( tools::translate( "CrossbowPluginConfigPanel", "Format for gdb: filename.{gdb|mdb}" ), box_ );
    new QLabel( tools::translate( "CrossbowPluginConfigPanel", "Format for sde connection: sde://user:password@host:port/db_name.sword" ), box_ );
    {
        new QLabel( tools::translate( "CrossbowPluginConfigPanel", "Geodatabase export:" ), box_ );
        geodatabase_ = new QLineEdit( "sde://user:password@host:port/sword_crossbow_db.sword", box_ );
    }
    {
        new QLabel( tools::translate( "CrossbowPluginConfigPanel", "Geodatabase population export:" ), box_ );
        population_ = new QLineEdit( box_ );
    }
    {
        new QLabel( tools::translate( "CrossbowPluginConfigPanel", "Geodatabase order import:" ), box_ );
        shared_ = new QLineEdit( box_ );
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
// Name: CrossbowPluginConfigPanel::IsChecked
// Created: JCR 2009-08-14
// -----------------------------------------------------------------------------
bool CrossbowPluginConfigPanel::IsChecked() const
{
    return box_->isChecked();
}
 

// -----------------------------------------------------------------------------
// Name: CrossbowPluginConfigPanel::Commit
// Created: SBO 2008-03-05
// -----------------------------------------------------------------------------
void CrossbowPluginConfigPanel::Commit( const std::string& exercise, const std::string& session )
{
    if( IsChecked() )
    {
        frontend::CreateSession action( config_, exercise, session );
        action.SetOption( "session/config/simulation/dispatcher/@embedded", false );
        action.SetOption( "session/config/dispatcher/plugins/crossbow/@geodatabase", geodatabase_->text() );
        action.SetOption( "session/config/dispatcher/plugins/crossbow/@geodatabase-population", population_->text() );
        action.SetOption( "session/config/dispatcher/plugins/crossbow/@geodatabase-shared", shared_->text() );
    }
}
