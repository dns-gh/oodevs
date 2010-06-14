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
#pragma warning( push )
#pragma warning( disable: 4127 4244 4245 4996 )
#include <boost/filesystem/operations.hpp>
#include <boost/filesystem/convenience.hpp>
#pragma warning( pop )
#include <xeumeuleu/xml.hpp>
#include <qstringlist.h>

namespace bfs = boost::filesystem;

// -----------------------------------------------------------------------------
// Name: SessionList constructor
// Created: SBO 2009-12-13
// -----------------------------------------------------------------------------
SessionList::SessionList( QWidget* parent, const tools::GeneralConfig& config )
    : QVBox( parent )
    , config_( config )
{
    setSpacing( 5 );
    setBackgroundOrigin( QWidget::WindowOrigin );
    {
        QLabel* label = new QLabel( tools::translate( "SessionList", "Session:" ), this );
        label->setBackgroundOrigin( QWidget::WindowOrigin );
        list_ = new QListBox( this );
        connect( list_, SIGNAL( highlighted( int ) ), this, SLOT( SelectSession( int ) ) );
    }
    {
        comments_ = new QTextEdit( this );
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
// Name: SessionList::Update
// Created: SBO 2009-12-13
// -----------------------------------------------------------------------------
void SessionList::Update( const QString& exercise )
{
    if( exercise_ != exercise.ascii() )
    {
        exercise_ = exercise;
        comments_->clear();
        list_->clear();
        list_->insertStringList( frontend::commands::ListSessions( config_, exercise.ascii() ) );
        list_->setSelected( 0, true );
    }
}

// -----------------------------------------------------------------------------
// Name: SessionList::SelectSession
// Created: SBO 2009-12-13
// -----------------------------------------------------------------------------
void SessionList::SelectSession( int index )
{
    const QString session = index < int( list_->count() ) ? list_->text( index ) : "";
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
        xml::xifstream xis( ( bfs::path( config_.BuildSessionDir( exercise_.ascii(), session.ascii() ) ) / "session.xml" ).native_file_string() );
        xis >> xml::start( "session" )
                >> xml::start( "meta" )
                    >> xml::content( "name", name )
                    >> xml::content( "comment", comment )
                    >> xml::content( "date", date );
        comments_->setShown( !name.empty() || !comment.empty() );
        comments_->setText( QString( "<b>%1</b><br><i>%2</i><br>%3" ).arg( name.c_str() ).arg( date.c_str() ).arg( comment.c_str() ) );
    }
    catch( ... )
    {
        comments_->hide();
    }
}
