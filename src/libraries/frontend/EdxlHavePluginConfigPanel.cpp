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
#pragma warning( push, 0 )
#include <QtGui/qlineedit.h>
#include <QtGui/qspinbox.h>
#include <Qt3Support/q3groupbox.h>
#include <Qt3Support/q3datetimeedit.h>
#include <QtGui/qlabel.h>
#include <QtGui/qcheckbox.h>
#pragma warning( pop )
#include <boost/filesystem.hpp>

using namespace frontend;

// -----------------------------------------------------------------------------
// Name: EdxlHavePluginConfigPanel constructor
// Created: SBO 2008-03-05
// -----------------------------------------------------------------------------
EdxlHavePluginConfigPanel::EdxlHavePluginConfigPanel( QWidget* parent, const tools::GeneralConfig& config )
    : PluginConfig_ABC( parent )
    , config_( config )
{
    mainBox_ = new Q3GroupBox( 1, Qt::Horizontal, this );
    mainBox_->setCheckable( true );
    mainBox_->setChecked( false );
    {
        Q3HBox* box = new Q3HBox( mainBox_ );
        hostLabel_ = new QLabel( box );
        host_ = new QLineEdit( "www.ewaphoenix.com", box );
    }
    {
        Q3HBox* box = new Q3HBox( mainBox_ );
        sslLabel_ = new QLabel( box );
        ssl_ = new QCheckBox( box );
        ssl_->setChecked( true );
    }
    {
        Q3HBox* box = new Q3HBox( mainBox_ );
        logLabel_ = new QLabel( box );
        log_ = new QCheckBox( box );
        log_->setChecked( true );
    }
    {
        servicesBox_ = new Q3GroupBox( 2, Qt::Horizontal, mainBox_ );
        {
            initLabel_ = new QLabel( servicesBox_ );
            initializeServiceURI_ = new QLineEdit( "/EWAPhoenix-BedTracking/XMLReceive.php?message=initialize", servicesBox_ );
        }
        {
            updateLabel_ = new QLabel( servicesBox_ );
            updateServiceURI_ = new QLineEdit( "/EWAPhoenix-BedTracking/XMLReceive.php?message=update", servicesBox_ );

            frequencyLabel_ = new QLabel( servicesBox_ );
            frequency_ = new Q3TimeEdit( servicesBox_ );
            frequency_->setDisplay ( Q3TimeEdit::Hours | Q3TimeEdit::Minutes | Q3TimeEdit::Seconds  );
            frequency_->setTime( QTime().addSecs( 120 ) );
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
// Name: EdxlHavePluginConfigPanel::OnLanguageChanged
// Created: ABR 2011-11-10
// -----------------------------------------------------------------------------
void EdxlHavePluginConfigPanel::OnLanguageChanged()
{
    mainBox_->setTitle( tools::translate( "EdxlHavePluginConfigPanel", "Enable EDXL-HAVE exchange" ) );
    hostLabel_->setText( tools::translate( "EdxlHavePluginConfigPanel", "Host: " ) );
    sslLabel_->setText( tools::translate( "EdxlHavePluginConfigPanel", "Use SSL: " ) );
    logLabel_->setText( tools::translate( "EdxlHavePluginConfigPanel", "Log: " ) );

    servicesBox_->setTitle( tools::translate( "EdxlHavePluginConfigPanel", "Services configuration:" ) );
    initLabel_->setText( tools::translate( "EdxlHavePluginConfigPanel", "Initialization service: " ) );
    updateLabel_->setText( tools::translate( "EdxlHavePluginConfigPanel", "Update service: " ) );
    frequencyLabel_->setText( tools::translate( "EdxlHavePluginConfigPanel", "Frequency: " ) );
}

// -----------------------------------------------------------------------------
// Name: EdxlHavePluginConfigPanel::GetName
// Created: SBO 2011-05-09
// -----------------------------------------------------------------------------
QString EdxlHavePluginConfigPanel::GetName() const
{
    return tools::translate( "EdxlHavePluginConfigPanel", "EDXL-HAVE" );
}

// -----------------------------------------------------------------------------
// Name: EdxlHavePluginConfigPanel::Commit
// Created: SBO 2008-03-05
// -----------------------------------------------------------------------------
void EdxlHavePluginConfigPanel::Commit( const std::string& exercise, const std::string& /*session*/ )
{
    boost::filesystem::path exerciceDir( config_.GetExerciseDir( exercise ) );
    boost::filesystem::path file = exerciceDir / "edxl.ini";
    if( mainBox_->isChecked() )
    {
        std::ofstream of( file.string().c_str() );
        of << "edxl.host=" << host_->text().ascii() << std::endl
           << "edxl.initialization.serviceURI=" << initializeServiceURI_->text() << std::endl
           << "edxl.update.serviceURI=" << updateServiceURI_->text() << std::endl
           << "edxl.update.frequency=" << QTime().secsTo( frequency_->time() ) << 's' << std::endl
           << "edxl.ssl=true" << std::endl
           << "sword.waitconnection=true" << std::endl
           << "log=true" << std::endl
           << "log.file=" << exerciceDir / "edxl.log" << std::endl;
    }
    else
        boost::filesystem::remove( file );
}
