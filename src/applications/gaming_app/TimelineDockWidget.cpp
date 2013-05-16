// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2013 MASA Group
//
// *****************************************************************************

#include "gaming_app_pch.h"
#include "TimelineDockWidget.h"
#include "Config.h"
#include "timeline/api.h"

// -----------------------------------------------------------------------------
// Name: TimelineDockWidget constructor
// Created: ABR 2013-05-14
// -----------------------------------------------------------------------------
TimelineDockWidget::TimelineDockWidget( QWidget* parent, kernel::Controllers& controllers, const Config& config )
    : gui::RichDockWidget( controllers, parent, "timeline-dock-widget" )
    , cfg_( new timeline::Configuration() )
{
    setCaption( tr( "Actions timeline" ) );

    QWidget* mainWidget = new QWidget( this );
    setWidget( mainWidget );

    // Configuration
    cfg_->widget = mainWidget;
    cfg_->url = "http://" + config.GetTimelineUrl();
    cfg_->debug_port = config.GetTimelineDebugPort();
    cfg_->rundir = "cef";
    cfg_->binary = "cef/timeline_client.exe";
    cfg_->external = true;

    if( !cfg_->binary.IsRegularFile() )
    {
        QMessageBox::warning( 0, tr( "Warning" ), tr( "Invalid timeline binary '%1'" ).arg( QString::fromStdWString( cfg_->binary.ToUnicode() ) ) );
        return;
    }
}

// -----------------------------------------------------------------------------
// Name: TimelineDockWidget destructor
// Created: ABR 2013-05-14
// -----------------------------------------------------------------------------
TimelineDockWidget::~TimelineDockWidget()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: TimelineDockWidget::Connect
// Created: ABR 2013-05-15
// -----------------------------------------------------------------------------
void TimelineDockWidget::Connect()
{
    server_ = timeline::MakeServer( *cfg_ );
}

// -----------------------------------------------------------------------------
// Name: TimelineDockWidget::Disconnect
// Created: ABR 2013-05-15
// -----------------------------------------------------------------------------
void TimelineDockWidget::Disconnect()
{
    if( server_.get() )
        server_.reset();
}
