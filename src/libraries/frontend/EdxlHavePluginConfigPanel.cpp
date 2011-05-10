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
#include <boost/filesystem.hpp>

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
        host_ = Style( new QLineEdit( "www.ewaphoenix.com", box ) );
    }
    {
        QHBox* box = Style( new QHBox( box_ ) );
        Style( new QLabel( tools::translate( "EdxlHavePluginConfigPanel", "Use SSL: " ), box ) );
        ssl_ = Style( new QCheckBox( box ) );
        ssl_->setChecked( true );
    }
    {
        QHBox* box = Style( new QHBox( box_ ) );
        Style( new QLabel( tools::translate( "EdxlHavePluginConfigPanel", "Log: " ), box ) );
        log_ = Style( new QCheckBox( box ) );
        log_->setChecked( true );
    }
    {
        QGroupBox* services = new QGroupBox( 2, Horizontal, tools::translate( "EdxlHavePluginConfigPanel", "Services configuration:" ), box_ );
        {
            Style( new QLabel( tools::translate( "EdxlHavePluginConfigPanel", "Initialization service: " ), services ) );
            initializeServiceURI_ = Style( new QLineEdit( "/EWAPhoenix-BedTracking/XMLReceive.php?message=initialize", services ) );
        }
        {
            Style( new QLabel( tools::translate( "EdxlHavePluginConfigPanel", "Update service: " ), services ) );
            updateServiceURI_ = Style( new QLineEdit( "/EWAPhoenix-BedTracking/XMLReceive.php?message=update", services ) );

            Style( new QLabel( tools::translate( "EdxlHavePluginConfigPanel", "Frequency: " ), services ) );
            frequency_ = new QTimeEdit( services );
            frequency_->setDisplay ( QTimeEdit::Hours | QTimeEdit::Minutes | QTimeEdit::Seconds  );
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
    if( box_->isChecked() )
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
