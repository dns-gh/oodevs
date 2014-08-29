// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 MASA Group
//
// *****************************************************************************

#include "selftraining_app_pch.h"
#include "LogProgressPage.h"
#include "moc_LogProgressPage.cpp"
#include "Application.h"
#include "frontend/Process_ABC.h"
#include "clients_kernel/Controllers.h"
#include "clients_kernel/Tools.h"

LogProgressPage::LogProgressPage( Application& app, QStackedWidget* pages, Page_ABC& previous )
    : ContentPage( pages, previous, eButtonCancel | eButtonBack )
    , app_       ( app )
{
    EnableButton( eButtonBack, false );
    log_ = new QListWidget();
    log_->setFont( QFont( "Calibri", 12, QFont::Bold ) );

    QWidget* box = new QWidget( this );
    QVBoxLayout* boxLayout = new QVBoxLayout( box );
    boxLayout->setAlignment( Qt::AlignCenter );
    boxLayout->addWidget( log_ );
    boxLayout->setSpacing( 10 );
    AddContent( box );
}

LogProgressPage::~LogProgressPage()
{
}

void LogProgressPage::Start( const std::function< void() >& backCallback )
{
    EnableButton( eButtonCancel, true );
    EnableButton( eButtonBack, false );
    backCallback_ = backCallback;
    log_->clear();
    show();
}

void LogProgressPage::Log( const std::string& line )
{
    QMetaObject::invokeMethod( this, "OnLog",
            Q_ARG( QString, QString::fromStdString( line ) ) );
}

void LogProgressPage::OnLog( const QString& line )
{
    log_->addItem( line );
    log_->scrollToBottom();
}

void LogProgressPage::NotifyDone( const std::string& error )
{
    QMetaObject::invokeMethod( this, "OnNotifyDone", Q_ARG( QString, QString( error.c_str() ) ) );
}

void LogProgressPage::OnNotifyDone( const QString& message )
{
    EnableButton( eButtonCancel, false );
    EnableButton( eButtonBack, true );
    if( message.size() > 0 )
        QMessageBox::critical( 0, tools::translate( "Application", "Error" ), message );
}

void LogProgressPage::OnBack()
{
    EnableButton( eButtonCancel, false );
    EnableButton( eButtonBack, true );
    if( backCallback_ )
    {
        backCallback_();
        backCallback_ = nullptr;
    }
    app_.GetMainWindow()->show();
    app_.GetMainWindow()->activateWindow();
    Previous();
}

void LogProgressPage::OnCancel()
{
    OnBack();
}

