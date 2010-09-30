// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "frontend_pch.h"
#include "DisPluginConfigPanel.h"
#include "CreateSession.h"
#include "tools/GeneralConfig.h"
#include "clients_gui/Tools.h"
#include <qlineedit.h>
#include <qspinbox.h>
#include <qgroupbox.h>
#include <qcheckbox.h>
#include <qlabel.h>

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
// Name: DisPluginConfigPanel constructor
// Created: SBO 2008-03-05
// -----------------------------------------------------------------------------
DisPluginConfigPanel::DisPluginConfigPanel( QWidget* parent, const tools::GeneralConfig& config )
    : PluginConfig_ABC( parent )
    , config_( config )
{
    setMargin( 5 );
    setBackgroundOrigin( QWidget::WindowOrigin );
    box_ = Style( new QGroupBox( 2, Horizontal, tools::translate( "DisPluginConfigPanel", "Enable DIS export" ), this ) );
    box_->setCheckable( true );
    box_->setChecked( false );
    {
        Style( new QLabel( tools::translate( "DisPluginConfigPanel", "Disaggregate units:" ), box_ ) );
        tic_ = Style( new QCheckBox( box_ ) );
        tic_->setChecked( false );
    }
    {
        Style( new QLabel( tools::translate( "DisPluginConfigPanel", "Host: " ), box_ ) );
        QHBox* box = Style( new QHBox( box_ ) );
        server_ = Style( new QLineEdit( "localhost", box ) );
        Style( new QLabel( " : ", box ) );
        port_ = Style( new QSpinBox( 0, 65535, 1, box ) );
        port_->setValue( 12000 );
    }
    {
        Style( new QLabel( tools::translate( "DisPluginConfigPanel", "Site Id: " ), box_ ) );
        site_ = Style( new QSpinBox( 0, 32767, 1, box_ ) ); // unsigned short
        site_->setValue( 1 );
    }
    {
        Style( new QLabel( tools::translate( "DisPluginConfigPanel", "Application Id: " ), box_ ) );
        application_ = Style( new QSpinBox( 0, 32767, 1, box_ ) ); // unsigned short
        application_->setValue( 1 );
    }
    {
        Style( new QLabel( tools::translate( "DisPluginConfigPanel", "Exercise Id: " ), box_ ) );
        exercise_ = Style( new QSpinBox( 0, 255, 1, box_ ) ); // unsigned char
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
        action.SetOption( "session/config/dispatcher/plugins/dis/@server", server_->text() );
        action.SetOption( "session/config/dispatcher/plugins/dis/@port", port_->value() );
        action.SetOption( "session/config/dispatcher/plugins/dis/@site", site_->value() );
        action.SetOption( "session/config/dispatcher/plugins/dis/@application", application_->value() );
        action.SetOption( "session/config/dispatcher/plugins/dis/@exercise", exercise_->value() );
        if( tic_->isChecked() )
           action.SetOption( "session/config/dispatcher/plugins/tic/@enable", true );
        action.Commit();
    }
}
