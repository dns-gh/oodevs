// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "frontend_pch.h"
#include "EdxlHavePluginConfigPanel.h"
#include "CreateSession.h"
#include "tools/GeneralConfig.h"
#include "clients_gui/Tools.h"
#include <qlineedit.h>
#include <qspinbox.h>
#include <qgroupbox.h>
#include <qdatetimeedit.h>
#include <qlabel.h>
#include <qcheckbox.h>


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
// Name: EdxlHavePluginConfigPanel constructor
// Created: SBO 2008-03-05
// -----------------------------------------------------------------------------
EdxlHavePluginConfigPanel::EdxlHavePluginConfigPanel( QWidget* parent, const tools::GeneralConfig& config )
    : PluginConfig_ABC( parent )
    , config_( config )
{
    box_ = Style( new QGroupBox( 1, Horizontal, tools::translate( "EdxlHavePluginConfigPanel", "Enable EDXL-HAVE exchange" ), this ) );
    box_->setCheckable( true );
    box_->setChecked( false );
    {
        QHBox* box = Style( new QHBox( box_ ) );
        Style( new QLabel( tools::translate( "EdxlHavePluginConfigPanel", "Host: " ), box ) );
        host_ = Style( new QLineEdit( "localhost", box ) );
    }
    {
        QHBox* box = Style( new QHBox( box_ ) );
        Style( new QLabel( tools::translate( "EdxlHavePluginConfigPanel", "Use SSL: " ), box ) );
        ssl_ = Style( new QCheckBox( box ) );
        ssl_->setChecked( false );
    }
    {
        QHBox* box = Style( new QHBox( box_ ) );
        Style( new QLabel( tools::translate( "EdxlHavePluginConfigPanel", "Log: " ), box ) );
        log_ = Style( new QCheckBox( box ) );
        log_->setChecked( false );
    }
    {
        QGroupBox* services = new QGroupBox( 2, Horizontal, tools::translate( "EdxlHavePluginConfigPanel", "Services configuration:" ), box_ );
        {
            Style( new QLabel( tools::translate( "EdxlHavePluginConfigPanel", "Initialization service: " ), services ) );
            initializeServiceURI_ = Style( new QLineEdit( "/initialize", services ) );
        }
        {
            Style( new QLabel( tools::translate( "EdxlHavePluginConfigPanel", "Update service: " ), services ) );
            updateServiceURI_ = Style( new QLineEdit( "/update", services ) );

            Style( new QLabel( tools::translate( "EdxlHavePluginConfigPanel", "Frequency: " ), services ) );
            frequency_ = new QTimeEdit( services );
            frequency_->setDisplay ( QTimeEdit::Hours | QTimeEdit::Minutes | QTimeEdit::Seconds  );
            frequency_->setTime( QTime().addSecs( 3600 ) );
        }
    }
}

// -----------------------------------------------------------------------------
// Name: EdxlHavePluginConfigPanel destructor
// Created: SBO 2008-03-05
// -----------------------------------------------------------------------------
EdxlHavePluginConfigPanel::~EdxlHavePluginConfigPanel()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: EdxlHavePluginConfigPanel::Commit
// Created: SBO 2008-03-05
// -----------------------------------------------------------------------------
void EdxlHavePluginConfigPanel::Commit( const std::string& exercise, const std::string& session )
{
    if( box_->isChecked() )
    {
        frontend::CreateSession action( config_, exercise, session );
        action.SetOption( "session/config/dispatcher/plugins/edxl-have/services/@host", host_->text() );
        action.SetOption( "session/config/dispatcher/plugins/edxl-have/services/@type", ssl_->isChecked() ? "https" : "http" );
        action.SetOption( "session/config/dispatcher/plugins/edxl-have/services/@log", log_->isChecked() ? "true" : "false" );
        action.SetOption( "session/config/dispatcher/plugins/edxl-have/services/initialization/@serviceURI", initializeServiceURI_->text() );
        action.SetOption( "session/config/dispatcher/plugins/edxl-have/services/update/@serviceURI", updateServiceURI_->text() );
        action.SetOption( "session/config/dispatcher/plugins/edxl-have/services/update/@frequency", QString( "%1s" ).arg( QTime().secsTo( frequency_->time() ) ).ascii() );
        action.Commit();
    }
}
