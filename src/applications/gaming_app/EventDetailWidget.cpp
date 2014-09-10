// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2013 MASA Group
//
// *****************************************************************************

#include "gaming_app_pch.h"
#include "EventDetailWidget.h"
#include "clients_gui/EventViewState.h"
#include "clients_kernel/TimelineHelpers.h"
#include "protocol/Protocol.h"
#include <timeline/api.h>
#include <tools/Base64Converters.h>

// -----------------------------------------------------------------------------
// Name: EventDetailWidget constructor
// Created: ABR 2013-05-31
// -----------------------------------------------------------------------------
EventDetailWidget::EventDetailWidget( gui::EventPresenter& presenter )
    : EventDefaultWidget_ABC( presenter )
{
    uuid_ = new QLineEdit();
    uuid_->setReadOnly( true );
    name_ = new QLineEdit();
    name_->setReadOnly( true );
    info_ = new QTextEdit();
    info_->setFixedHeight( 50 );
    info_->setReadOnly( true );
    begin_ = new QLineEdit();
    begin_->setReadOnly( true );
    end_ = new QLineEdit();
    end_->setReadOnly( true );
    parent_ = new QLineEdit();
    parent_->setReadOnly( true );
    done_ = new QCheckBox();
    done_->setEnabled( false );
    target_ = new QLineEdit();
    target_->setReadOnly( true );
    apply_ = new QCheckBox();
    apply_->setEnabled( false );
    metadata_ = new QTextEdit();
    metadata_->setReadOnly( true );
    metadata_->setFixedHeight( 30 );
    payloadBase64_ = new QTextEdit();
    payloadBase64_->setReadOnly( true );
    payloadBase64_->setFixedHeight( 50 );
    payloadString_ = new QTextEdit();
    payloadString_->setReadOnly( true );

    QGridLayout* gridLayout = new QGridLayout( 12, 2 );
    mainLayout_->addLayout( gridLayout );
    gridLayout->setMargin( 10 );
    gridLayout->setSpacing( 10 );
    gridLayout->addWidget( new QLabel( "UUID" ), 0, 0 );
    gridLayout->addWidget( uuid_, 0, 1 );
    gridLayout->addWidget( new QLabel( "Name" ), 1, 0 );
    gridLayout->addWidget( name_, 1, 1 );
    gridLayout->addWidget( new QLabel( "Infos" ), 2, 0 );
    gridLayout->addWidget( info_, 2, 1 );
    gridLayout->addWidget( new QLabel( "Begin" ), 3, 0 );
    gridLayout->addWidget( begin_, 3, 1 );
    gridLayout->addWidget( new QLabel( "End" ), 4, 0 );
    gridLayout->addWidget( end_, 4, 1 );
    gridLayout->addWidget( new QLabel( "Parent" ), 5, 0 );
    gridLayout->addWidget( parent_, 5, 1 );
    gridLayout->addWidget( new QLabel( "Metadata" ), 6, 0 );
    gridLayout->addWidget( metadata_, 6, 1 );
    gridLayout->addWidget( new QLabel( "Done" ), 7, 0 );
    gridLayout->addWidget( done_, 7, 1 );
    gridLayout->addWidget( new QLabel( "Action" ), 8, 0, 1, 2 );
    gridLayout->addWidget( new QLabel( "Target" ), 9, 0 );
    gridLayout->addWidget( target_, 9, 1 );
    gridLayout->addWidget( new QLabel( "Apply" ), 10, 0 );
    gridLayout->addWidget( apply_, 10, 1 );
    gridLayout->addWidget( new QLabel( "Base64 Payload" ), 11, 0 );
    gridLayout->addWidget( payloadBase64_, 11, 1 );
    gridLayout->addWidget( new QLabel( "String Payload" ), 12, 0 );
    gridLayout->addWidget( payloadString_, 12, 1 );
}

// -----------------------------------------------------------------------------
// Name: EventDetailWidget destructor
// Created: ABR 2013-05-31
// -----------------------------------------------------------------------------
EventDetailWidget::~EventDetailWidget()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: EventDetailWidget::Purge
// Created: ABR 2013-11-21
// -----------------------------------------------------------------------------
void EventDetailWidget::Purge()
{
    uuid_->clear();
    name_->clear();
    info_->clear();
    begin_->clear();
    end_->clear();
    parent_->clear();
    done_->setCheckState( Qt::Unchecked );
    metadata_->clear();
    target_->clear();
    apply_->setCheckState( Qt::Unchecked );
    payloadBase64_->clear();
    payloadString_->clear();
}

// -----------------------------------------------------------------------------
// Name: EventDetailWidget::Build
// Created: ABR 2013-11-21
// -----------------------------------------------------------------------------
void EventDetailWidget::Build( const gui::EventViewState& state )
{
    if( !state.event_ )
        return;
    const timeline::Event& timelineEvent = state.event_->GetEvent();
    uuid_->setText( QString::fromStdString( timelineEvent.uuid ) );
    name_->setText( QString::fromStdString( timelineEvent.name ) );
    info_->setText( QString::fromStdString( timelineEvent.info ) );
    begin_->setText( QString::fromStdString( timelineEvent.begin ) );
    end_->setText( QString::fromStdString( timelineEvent.end ) );
    parent_->setText( QString::fromStdString( timelineEvent.parent ) );
    metadata_->setText( QString::fromStdString( timelineEvent.metadata ) );
    done_->setCheckState( timelineEvent.done ? Qt::Checked : Qt::Unchecked );
    target_->setText( QString::fromStdString( timelineEvent.action.target ) );
    apply_->setCheckState( timelineEvent.action.apply ? Qt::Checked : Qt::Unchecked );
    if( !timelineEvent.action.payload.empty() )
    {
        payloadBase64_->setText( QString::fromStdString( tools::BinaryToBase64( timelineEvent.action.payload ) ) );
        if( timelineEvent.action.target == CREATE_EVENT_TARGET( EVENT_ORDER_PROTOCOL, EVENT_SIMULATION_SERVICE ) )
        {
            sword::ClientToSim msg;
            msg.ParsePartialFromString( timelineEvent.action.payload );
            payloadString_->setText( QString::fromStdString( msg.DebugString() ) );
        }
        else if ( timelineEvent.action.target == CREATE_EVENT_TARGET( EVENT_REPORT_PROTOCOL, EVENT_SIMULATION_SERVICE ) )
        {
            sword::SimToClient msg;
            msg.ParsePartialFromString( timelineEvent.action.payload );
            payloadString_->setText( QString::fromStdString( msg.DebugString() ) );
        }
        else
            payloadString_->setText( QString::fromStdString( timelineEvent.action.payload ) );
    }
    else
    {
        payloadBase64_->setText( "" );
        payloadString_->setText( "" );
    }
}
