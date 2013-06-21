// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2013 MASA Group
//
// *****************************************************************************

#include "gaming_app_pch.h"
#include "EventMultimediaWidget.h"
#include "moc_EventMultimediaWidget.cpp"
#include "clients_gui/FileDialog.h"
#include "ENT/ENT_Tr.h"
#include "gaming/Event.h"
#include "timeline/api.h"
#include "tools/GeneralConfig.h"

// -----------------------------------------------------------------------------
// Name: EventMultimediaWidget constructor
// Created: ABR 2013-05-30
// -----------------------------------------------------------------------------
EventMultimediaWidget::EventMultimediaWidget()
    : EventWidget_ABC()
{
    label_ = new QLineEdit();
    file_ = new QLineEdit();
    browse_ = new QPushButton( tr( "Browse..." ) );
    connect( browse_, SIGNAL( clicked() ), this, SLOT( OnBrowse() ) );
    profiles_ = new QComboBox();

    QGridLayout* gridLayout = new QGridLayout( 3, 3 );
    gridLayout->setMargin( 10 );
    gridLayout->setSpacing( 10 );

    gridLayout->addWidget( new QLabel( tr( "Label" ) ), 0, 0 );
    gridLayout->addWidget( label_, 0, 1, 1, 2 );
    gridLayout->addWidget( new QLabel( tr( "File" ) ), 1, 0 );
    gridLayout->addWidget( file_, 1, 1 );
    gridLayout->addWidget( browse_, 1, 2 );
    gridLayout->addWidget( new QLabel( tr( "Profiles" ) ), 2, 0 );
    gridLayout->addWidget( profiles_, 2, 1, 1, 2 );

    mainLayout_->addLayout( gridLayout );
    mainLayout_->addStretch( 1 );
}

// -----------------------------------------------------------------------------
// Name: EventMultimediaWidget destructor
// Created: ABR 2013-05-30
// -----------------------------------------------------------------------------
EventMultimediaWidget::~EventMultimediaWidget()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: EventMultimediaWidget::Fill
// Created: ABR 2013-05-30
// -----------------------------------------------------------------------------
void EventMultimediaWidget::Fill( const Event& event )
{
    file_->setText( "" );
    label_->setText( QString::fromStdString( event.GetEvent().name ) );
}

// -----------------------------------------------------------------------------
// Name: EventMultimediaWidget::Commit
// Created: ABR 2013-05-30
// -----------------------------------------------------------------------------
void EventMultimediaWidget::Commit( timeline::Event& event ) const
{
    event.name = label_->text().toStdString();
    event.info = file_->text().toStdString();
    event.action.target = "sword://multimedia";
}

// -----------------------------------------------------------------------------
// Name: EventMultimediaWidget::InternalCheckValidity
// Created: ABR 2013-05-31
// -----------------------------------------------------------------------------
bool EventMultimediaWidget::IsValid() const
{
    tools::Path file = tools::Path::FromUnicode( file_->text().toStdWString() );
    return file.Exists() && file.IsRegularFile();
}

// -----------------------------------------------------------------------------
// Name: EventMultimediaWidget::OnBrowse
// Created: ABR 2013-05-31
// -----------------------------------------------------------------------------
void EventMultimediaWidget::OnBrowse()
{
    tools::Path file = gui::FileDialog::getOpenFileName( QApplication::activeModalWidget(), tr( "Select output directory" ), tools::GeneralConfig::BuildResourceChildFile( "dummy" ).Parent() );
    file_->setText( file.ToUTF8().c_str() );
}
