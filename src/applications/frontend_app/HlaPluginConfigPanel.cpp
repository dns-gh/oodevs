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
#include <qlineedit.h>

// -----------------------------------------------------------------------------
// Name: HlaPluginConfigPanel constructor
// Created: SBO 2008-03-05
// -----------------------------------------------------------------------------
HlaPluginConfigPanel::HlaPluginConfigPanel( QWidget* parent, const tools::GeneralConfig& config )
    : QHBox( parent )
    , config_( config )
{
    box_ = new QGroupBox( 2, Horizontal, tr( "HLA export plugin configuration" ), this );
    box_->setCheckable( true );
    box_->setChecked( false );
    {
        new QLabel( tr( "Federation: " ), box_ );
        federation_ = new QLineEdit( box_ );
    }
    {
        new QLabel( tr( "Federate name: " ), box_ );
        name_ = new QLineEdit( box_ );
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
