// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "frontend_app_pch.h"
#include "HlaPluginConfigPanel.h"
#include "tools/GeneralConfig.h"
#include "frontend/CreateSession.h"
#include "clients_gui/Tools.h"
#include <qlineedit.h>
#include <qgroupbox.h>
#include <qlabel.h>

// -----------------------------------------------------------------------------
// Name: HlaPluginConfigPanel constructor
// Created: SBO 2008-03-05
// -----------------------------------------------------------------------------
HlaPluginConfigPanel::HlaPluginConfigPanel( QWidget* parent, const tools::GeneralConfig& config )
    : QHBox( parent )
    , config_( config )
{
    box_ = new QGroupBox( 2, Horizontal, tools::translate( "HlaPluginConfigPanel", "Enable HLA RPR-FOM export" ), this );
    box_->setCheckable( true );
    box_->setChecked( false );
    {
        new QLabel( tools::translate( "HlaPluginConfigPanel", "Federation: " ), box_ );
        federation_ = new QLineEdit( "MyFederation", box_ );
    }
    {
        new QLabel( tools::translate( "HlaPluginConfigPanel", "Federate name: " ), box_ );
        name_ = new QLineEdit( tools::translate( "Application", "Sword Officer Training" ), box_ );
    }
}

// -----------------------------------------------------------------------------
// Name: HlaPluginConfigPanel destructor
// Created: SBO 2008-03-05
// -----------------------------------------------------------------------------
HlaPluginConfigPanel::~HlaPluginConfigPanel()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: HlaPluginConfigPanel::Commit
// Created: SBO 2008-03-05
// -----------------------------------------------------------------------------
void HlaPluginConfigPanel::Commit( const std::string& exercise, const std::string& session )
{
    if( box_->isChecked() )
    {
        frontend::CreateSession action( config_, exercise, session );
        {
            action.SetOption( "session/config/dispatcher/plugins/hla/@federation", federation_->text() );
            action.SetOption( "session/config/dispatcher/plugins/hla/@name", name_->text() );
        }
    }
}
