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
#include "clients_gui/Tools.h"
#include "tools/GeneralConfig.h"
#include "tools/Loader_ABC.h"
#include <boost/filesystem/operations.hpp>
#include <boost/filesystem/convenience.hpp>
#include <xeumeuleu/xml.hpp>

namespace bfs = boost::filesystem;

// -----------------------------------------------------------------------------
// Name: SessionList constructor
// Created: SBO 2009-12-13
// -----------------------------------------------------------------------------
SessionList::SessionList( QWidget* parent, const tools::GeneralConfig& config, const tools::Loader_ABC& fileLoader )
    : gui::LanguageChangeObserver_ABC< Q3VBox >( parent )
    , config_( config )
    , fileLoader_( fileLoader )
{
    setSpacing( 5 );
    {
        sessionLabel_ = new QLabel( this );
        list_ = new QListWidget( this );
        list_->setFont( QFont( "Calibri", 12, QFont::Bold ) );
        connect( list_, SIGNAL( currentRowChanged( int ) ), this, SLOT( SelectSession( int ) ) );
    }
    {
        comments_ = new Q3TextEdit( this );
        comments_->setMaximumHeight( 200 );
        comments_->setReadOnly( true );
        comments_->hide();
    }
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
void SessionList::Update( const QString& exercise )
{
    if( exercise_ != exercise )
    {
        exercise_ = exercise;
        comments_->clear();
        list_->clear();
        list_->addItems( frontend::commands::ListSessions( config_, exercise.toAscii().constData() ) );
        list_->setCurrentRow( 0 );
    }
}

// -----------------------------------------------------------------------------
// Name: SessionList::SelectSession
// Created: SBO 2009-12-13
// -----------------------------------------------------------------------------
void SessionList::SelectSession( int index )
{
    const QString session = index < int( list_->count() ) ? list_->item( index )->text() : "";
    ReadComments( session );
    emit Select( session );
}

// -----------------------------------------------------------------------------
// Name: SessionList::ReadComments
// Created: SBO 2009-12-13
// -----------------------------------------------------------------------------
void SessionList::ReadComments( const QString& session )
{
    try
    {
        std::string date, name, comment;
        std::auto_ptr< xml::xistream > xis = fileLoader_.LoadFile( (bfs::path( config_.BuildSessionDir( exercise_.toAscii().constData(), session.toAscii().constData() ) ) / "session.xml" ).string() );
        *xis >> xml::start( "session" )
                >> xml::start( "meta" );
        if( xis->has_child( "name" ) )
        {
            *xis >> xml::start( "name" );
            if( xis->has_content() )
                *xis >> name;
            *xis >> xml::end;
        }
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
