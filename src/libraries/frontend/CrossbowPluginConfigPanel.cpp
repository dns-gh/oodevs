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
#include <qvbox.h>
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
    setMargin( 5 );
    setBackgroundOrigin( QWidget::WindowOrigin );
    box_ = Style( new QGroupBox( 2, Horizontal, tools::translate( "CrossbowPluginConfigPanel", "Enable Crossbow export" ), this ) );
    box_->setCheckable( true );
    box_->setChecked( false );
    QVBox* box = Style( new QVBox( box_ ) );
    {
        QGroupBox* geometry = new QGroupBox( 1, Horizontal, tools::translate( "CrossbowPluginConfigPanel", "Feature database (manage geometry information):" ), box );
        Style( new QLabel( tools::translate( "CrossbowPluginConfigPanel", "Connection URL:" ), geometry ) );
        featureDb_ = Style( new QLineEdit( "sde://user:password@host:port/sword_crossbow_db.sword", geometry ) );
        QToolTip::add( featureDb_, tools::translate( "CrossbowPluginConfigPanel", "GDB: filename.{gdb|mdb}\nSDE connection: sde://user:password@host:port/db_name.sword" ) );
    }
    {
        QGroupBox* flat = new QGroupBox( 1, Horizontal, tools::translate( "CrossbowPluginConfigPanel", "Flat database (manage no geometry information):" ), box );
        Style( new QLabel( tools::translate( "CrossbowPluginConfigPanel", "Connection URL:" ), flat ) );
        flatDb_ = Style( new QLineEdit( "postgres://user:password@host:port/sword_crossbow_db.sword", flat ) );
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
        action.SetOption( "session/config/dispatcher/plugins/crossbow/geometry/@connection", featureDb_->text() );
        action.SetOption( "session/config/dispatcher/plugins/crossbow/flat/@connection", flatDb_->text() );
        action.SetOption( "session/config/simulation/dispatcher/@embedded", true );
        // action.SetOption( "session/config/simulation/dispatcher/@path", "./dispatcher/" );
        action.Commit();
    }
}
