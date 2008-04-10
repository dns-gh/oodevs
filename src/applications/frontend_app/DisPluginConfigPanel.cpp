// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "frontend_app_pch.h"
#include "DisPluginConfigPanel.h"
#include "tools/GeneralConfig.h"
#include "frontend/CreateSession.h"
#include "clients_gui/Tools.h"
#include <qlineedit.h>
#include <qspinbox.h>

// -----------------------------------------------------------------------------
// Name: DisPluginConfigPanel constructor
// Created: SBO 2008-03-05
// -----------------------------------------------------------------------------
DisPluginConfigPanel::DisPluginConfigPanel( QWidget* parent, const tools::GeneralConfig& config )
    : QHBox( parent )
    , config_( config )
{
    box_ = new QGroupBox( 2, Horizontal, tools::translate( "DisPluginConfigPanel", "Enable DIS export" ), this );
    box_->setCheckable( true );
    box_->setChecked( false );
    {
        new QLabel( tools::translate( "DisPluginConfigPanel", "Host: " ), box_ );
        QHBox* box = new QHBox( box_ );
        server_ = new QLineEdit( "localhost", box );
        new QLabel( " : ", box );
        port_ = new QSpinBox( 0, 65535, 1, box );
        port_->setValue( 12000 );
    }
    {
        new QLabel( tools::translate( "DisPluginConfigPanel", "Site Id: " ), box_ );
        site_ = new QSpinBox( box_ );
        site_->setValue( 1 );
    }
    {
        new QLabel( tools::translate( "DisPluginConfigPanel", "Application Id: " ), box_ );
        application_ = new QSpinBox( box_ );
        application_->setValue( 1 );
    }
    {
        new QLabel( tools::translate( "DisPluginConfigPanel", "Exercise Id: " ), box_ );
        exercise_ = new QSpinBox( box_ );
        exercise_->setValue( 1 );
    }
}

// -----------------------------------------------------------------------------
// Name: DisPluginConfigPanel destructor
// Created: SBO 2008-03-05
// -----------------------------------------------------------------------------
DisPluginConfigPanel::~DisPluginConfigPanel()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: DisPluginConfigPanel::Commit
// Created: SBO 2008-03-05
// -----------------------------------------------------------------------------
void DisPluginConfigPanel::Commit( const std::string& exercise, const std::string& session )
{
    if( box_->isChecked() )
    {
        frontend::CreateSession action( config_, exercise, session );
        {
            action.SetOption( "session/config/dispatcher/plugins/dis/@server", server_->text() );
            action.SetOption( "session/config/dispatcher/plugins/dis/@port", port_->value() );
            action.SetOption( "session/config/dispatcher/plugins/dis/@site", site_->value() );
            action.SetOption( "session/config/dispatcher/plugins/dis/@application", application_->value() );
            action.SetOption( "session/config/dispatcher/plugins/dis/@exercise", exercise_->value() );
        }
    }
}
