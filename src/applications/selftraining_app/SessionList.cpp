// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#include "selftraining_app_pch.h"
#include "SessionList.h"
#include "moc_SessionList.cpp"
#include "frontend/commands.h"
#include "clients_kernel/Tools.h"
#include "tools/GeneralConfig.h"
#include "tools/Loader_ABC.h"
#include <xeumeuleu/xml.hpp>

namespace fc = frontend::commands;

// -----------------------------------------------------------------------------
// Name: SessionList constructor
// Created: SBO 2009-12-13
// -----------------------------------------------------------------------------
SessionList::SessionList( QWidget* parent, const tools::GeneralConfig& config, const tools::Loader_ABC& fileLoader )
    : gui::WidgetLanguageObserver_ABC< QWidget >( parent )
    , config_( config )
    , fileLoader_( fileLoader )
{
    {
        sessionLabel_ = new QLabel();
        list_ = new QListWidget();
        list_->setFont( QFont( "Calibri", 12, QFont::Bold ) );
        connect( list_, SIGNAL( currentRowChanged( int ) ), this, SLOT( SelectSession( int ) ) );
    }
    {
        comments_ = new QTextEdit();
        comments_->setMaximumHeight( 200 );
        comments_->setReadOnly( true );
        comments_->hide();
    }
    QVBoxLayout* layout = new QVBoxLayout( this );
    layout->setSpacing( 5 );
    layout->addWidget( sessionLabel_ );
    layout->addWidget( list_ );
    layout->addWidget( comments_ );

    parent->layout()->addWidget( this );
}

// -----------------------------------------------------------------------------
// Name: SessionList destructor
// Created: SBO 2009-12-13
// -----------------------------------------------------------------------------
SessionList::~SessionList()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: SessionList::OnLanguageChanged
// Created: ABR 2011-11-09
// -----------------------------------------------------------------------------
void SessionList::OnLanguageChanged()
{
    sessionLabel_->setText( tools::translate( "SessionList", "Session:" ) );
}

// -----------------------------------------------------------------------------
// Name: SessionList::Update
// Created: SBO 2009-12-13
// -----------------------------------------------------------------------------
void SessionList::Update( const tools::Path& exercise )
{
    if( exercise_ != exercise )
    {
        exercise_ = exercise;
        comments_->clear();
        list_->clear();
        list_->addItems( fc::PathListToQStringList( fc::ListSessions( config_, exercise, true ) ) );
        list_->setCurrentRow( 0 );
    }
}

// -----------------------------------------------------------------------------
// Name: SessionList::SelectSession
// Created: SBO 2009-12-13
// -----------------------------------------------------------------------------
void SessionList::SelectSession( int index )
{
    if( index < 0 || index >= list_->count() )
        return;
    const tools::Path session = tools::Path::FromUnicode( list_->item( index )->text().toStdWString() );
    ReadComments( session );
    emit Select( session );
}

// -----------------------------------------------------------------------------
// Name: SessionList::ReadComments
// Created: SBO 2009-12-13
// -----------------------------------------------------------------------------
void SessionList::ReadComments( const tools::Path& session )
{
    try
    {
        std::string date, name, comment;
        std::unique_ptr< xml::xistream > xis = fileLoader_.LoadFile( config_.BuildSessionDir( exercise_, session ) / "session.xml" );
        *xis >> xml::start( "session" )
                >> xml::start( "meta" );
        if( xis->has_child( "comment" ) )
        {
            *xis >> xml::start( "comment" );
            if( xis->has_content() )
                *xis >> comment;
            *xis >> xml::end;
        }
        if(
            xis->has_child( "date" ) )
        {
            *xis >> xml::start( "date" );
            if( xis->has_content() )
                *xis >> date;
            *xis >> xml::end;
        }
        comments_->setVisible( !name.empty() || !comment.empty() );
        comments_->setText( QString( "<b>%1</b><br><i>%2</i><br>%3" ).arg( name.c_str() ).arg( date.c_str() ).arg( comment.c_str() ) );
    }
    catch( ... )
    {
        comments_->hide();
    }
}
