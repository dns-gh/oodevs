// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "frontend_pch.h"
#include "PositionsPluginConfigPanel.h"
#include "moc_PositionsPluginConfigPanel.cpp"
#include "clients_kernel/Tools.h"
#include "CreateSession.h"
#include <qgroupbox.h>
#include <qlabel.h>
#include <qhbox.h>
#include <qdatetimeedit.h>

namespace
{
    template< typename T >
    T* Style( T* widget )
    {
        widget->setBackgroundOrigin( QWidget::WindowOrigin );
        return widget;
    }
}

using namespace frontend;

// -----------------------------------------------------------------------------
// Name: PositionsPluginConfigPanel constructor
// Created: ABR 2011-04-04
// -----------------------------------------------------------------------------
PositionsPluginConfigPanel::PositionsPluginConfigPanel( QWidget* parent, const tools::GeneralConfig& config )
    : PluginConfig_ABC( parent )
    , config_( config )
{
    setMargin( 5 );
    setBackgroundOrigin( QWidget::WindowOrigin );

    {
        positionsSaverGroup_ = Style( new QGroupBox( 1, Qt::Horizontal, tools::translate( "PositionsPluginConfigPanel", "Positions logger: " ), this ) );
        positionsSaverGroup_->setCheckable( true );
        positionsSaverGroup_->setChecked( false );
        {
            QHBox* frequencyBox = Style( new QHBox( positionsSaverGroup_ ) );
            Style( new QLabel( tools::translate( "PositionsPluginConfigPanel", "Frequency:" ), frequencyBox ) );
            frequency_ = Style( new QTimeEdit( frequencyBox ) );
            frequency_->setDisplay ( QTimeEdit::Hours | QTimeEdit::Minutes | QTimeEdit::Seconds  );
            frequency_->setTime( QTime().addSecs( 600 ) );
        }
    }
}

// -----------------------------------------------------------------------------
// Name: PositionsPluginConfigPanel destructor
// Created: ABR 2011-04-04
// -----------------------------------------------------------------------------
PositionsPluginConfigPanel::~PositionsPluginConfigPanel()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PositionsPluginConfigPanel::Commit
// Created: ABR 2011-04-04
// -----------------------------------------------------------------------------
void PositionsPluginConfigPanel::Commit( const std::string& exercise, const std::string& session )
{
    if( positionsSaverGroup_->isChecked() )
    {
        frontend::CreateSession action( config_, exercise, session );
        action.SetOption( "session/config/dispatcher/plugins/position-saver/@frequency", QString( "%1s" ).arg( QTime().secsTo( frequency_->time() ) ).ascii() );
        action.Commit();
    }
}

