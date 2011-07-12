// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "frontend_pch.h"
#include "TimelinePluginConfigPanel.h"
#include "moc_TimelinePluginConfigPanel.cpp"
#include "CreateSession.h"
#include "clients_gui/Tools.h"
#include "tools/GeneralConfig.h"
#include <boost/filesystem/operations.hpp>
#include <boost/filesystem/convenience.hpp>
#include <qcheckbox.h>
#include <qdatetimeedit.h>
#include <qfiledialog.h>
#include <qgroupbox.h>
#include <qhbox.h>
#include <qlabel.h>
#include <qlineedit.h>
#include <qpushbutton.h>
#include <qspinbox.h>

namespace bfs = boost::filesystem;

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
// Name: TimelinePluginConfigPanel constructor
// Created: SBO 2008-03-05
// -----------------------------------------------------------------------------
TimelinePluginConfigPanel::TimelinePluginConfigPanel( QWidget* parent, const tools::GeneralConfig& config )
    : PluginConfig_ABC( parent )
    , config_( config )
{
    setMargin( 5 );
    setBackgroundOrigin( QWidget::WindowOrigin );
    box_ = Style( new QGroupBox( 1, Horizontal, tools::translate( "TimelinePluginConfigPanel", "Enable Timeline exchange" ), this ) );
    box_->setCheckable( true );
    box_->setChecked( false );
    {
        QHBox* box = Style( new QHBox( box_ ) );
        Style( new QLabel( tools::translate( "TimelinePluginConfigPanel", "Host: " ), box ) );
        host_ = Style( new QLineEdit( "localhost:8088", box ) );
    }
    {
        QHBox* box = Style( new QHBox( box_ ) );
        Style( new QLabel( tools::translate( "TimelinePluginConfigPanel", "Use SSL: " ), box ) );
        ssl_ = Style( new QCheckBox( box ) );
        ssl_->setChecked( false );
    }
    {
        QHBox* box = Style( new QHBox( box_ ) );
        Style( new QLabel( tools::translate( "TimelinePluginConfigPanel", "Log: " ), box ) );
        log_ = Style( new QCheckBox( box ) );
        log_->setChecked( false );
    }
    {
        QGroupBox* services = new QGroupBox( 1, Horizontal, tools::translate( "TimelinePluginConfigPanel", "Service configuration:" ), box_ );
        {
            QGroupBox* scenario = Style( new QGroupBox( 2, Horizontal, tools::translate( "TimelinePluginConfigPanel", "Scenario information:" ), services ) );
            Style( new QLabel( tools::translate( "TimelinePluginConfigPanel", "Scenario Id: " ), scenario ) );
            scenarioId_ = Style( new QLineEdit( "1", scenario ) );
            Style( new QLabel( tools::translate( "TimelinePluginConfigPanel", "Restart timeline scenario: " ), scenario ) );
            doRestart_ = Style( new QCheckBox( scenario ) );
            doRestart_->setChecked( false );
            Style( new QLabel( tools::translate( "TimelinePluginConfigPanel", "Actor: " ), scenario ) );
            {
                QHBox* box = new QHBox( scenario );
                Style( new QLabel( tools::translate( "TimelinePluginConfigPanel", "name: " ), box ) );
                actorName_ = Style( new QLineEdit( "SWORD", box ) );
                Style( new QLabel( tools::translate( "TimelinePluginConfigPanel", "id: " ), box ) );
                actorId_ = Style( new QLineEdit( "115276488", box ) );
            }
        }
        {
            QHBox* box = new QHBox( services );
            Style( new QLabel( tools::translate( "TimelinePluginConfigPanel", "Recorded orders: " ), box ) );
            order_ = new QPushButton( tools::translate( "TimelinePluginConfigPanel", "Select Orders..." ), box );
            connect( order_, SIGNAL( clicked() ), this, SLOT( OnOrderClicked() ) );
        }
    }
}

// -----------------------------------------------------------------------------
// Name: TimelinePluginConfigPanel destructor
// Created: SBO 2008-03-05
// -----------------------------------------------------------------------------
TimelinePluginConfigPanel::~TimelinePluginConfigPanel()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: TimelinePluginConfigPanel::OnOrderClicked
// Created: JCR 2010-09-09
// -----------------------------------------------------------------------------
void TimelinePluginConfigPanel::OnOrderClicked()
{
    const int max_size = 20;

    QString orderFile = QFileDialog::getOpenFileName(
                                QString( config_.GetExercisesDir().c_str() ), "Order File (*.ord)",
                                this, "Select",
                                "Select recorded orders" );
    if( !orderFile.isNull() && !orderFile.isEmpty() )
    {
        orderFile_ = orderFile;
        std::string::size_type size = orderFile_.length() - std::min( max_size, (int)orderFile_.length() );
        std::string msg( orderFile_, size, size );
        if( orderFile_.size() > max_size + 2 )
            order_->setText( std::string( ".." + msg ).c_str() );
        else
            order_->setText( msg.c_str() );
    }
}

// -----------------------------------------------------------------------------
// Name: TimelinePluginConfigPanel::GetName
// Created: SBO 2011-05-09
// -----------------------------------------------------------------------------
QString TimelinePluginConfigPanel::GetName() const
{
    return tools::translate( "TimelinePluginConfigPanel", "ERP/Timeline" );
}

// -----------------------------------------------------------------------------
// Name: TimelinePluginConfigPanel::Commit
// Created: SBO 2008-03-05
// -----------------------------------------------------------------------------
void TimelinePluginConfigPanel::Commit( const std::string& exercise, const std::string& session )
{
    if( box_->isChecked() )
    {
        frontend::CreateSession action( config_, exercise, session );
        action.SetOption( "session/config/dispatcher/plugins/timeline/@host", host_->text() );
        if( ssl_->isChecked() )
            action.SetOption( "session/config/dispatcher/plugins/timeline/@ssl", "true" );
        if( log_->isChecked() )
            action.SetOption( "session/config/dispatcher/plugins/timeline/@log", "true" );
        action.SetOption( "session/config/dispatcher/plugins/timeline/scenario/@id", scenarioId_->text() );
        if ( doRestart_->isChecked() )
            action.SetOption( "session/config/dispatcher/plugins/timeline/scenario/@restart", "true" );
        action.SetOption( "session/config/dispatcher/plugins/timeline/scenario/actor/@id", actorId_->text() );
        action.SetOption( "session/config/dispatcher/plugins/timeline/scenario/actor/@name", actorName_->text() );
        const bfs::path exerciseDir = bfs::path( config_.GetExerciseDir( exercise ), bfs::native );
        const bfs::path orderDir = bfs::path( orderFile_, bfs::native );
        if( exerciseDir.string() == std::string( orderDir.string(), 0, exerciseDir.string().size() ) )
            action.SetOption( "session/config/dispatcher/plugins/timeline/orders/@file", std::string( orderFile_, exerciseDir.string().size() + 1, orderFile_.size() - exerciseDir.string().size() - 1 ) );
        else
            action.SetOption( "session/config/dispatcher/plugins/timeline/orders/@file", orderFile_ );
        action.Commit();
    }
}
