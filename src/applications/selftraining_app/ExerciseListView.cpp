// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#include "selftraining_app_pch.h"
#include "ExerciseListView.h"
#include "frontend/Exercise_ABC.h"
#include "clients_gui/ValuedListItem.h"
#include "tools/GeneralConfig.h"
#include "tools/Loader_ABC.h"
#include <boost/filesystem.hpp>
#include <xeumeuleu/xml.hpp>

namespace bfs = boost::filesystem;

namespace
{
    class MyListViewItem : public gui::ValuedListItem
    {
    public:
        MyListViewItem( Q3ListView* parent, const std::string& fullpath )
            : gui::ValuedListItem( parent )
            , fullpath_( fullpath.c_str() ) {}
        MyListViewItem( Q3ListViewItem* parent, const std::string& fullpath )
            : gui::ValuedListItem( parent )
            , fullpath_( fullpath.c_str() ) {}
        virtual QString key( int column, bool /*ascending*/ ) const
        {
            if( column == 0 )
                return fullpath_.startsWith( "/" ) ? fullpath_ : text( 0 );
            return text( column );
        }
        const QString fullpath_;
    };
}

// -----------------------------------------------------------------------------
// Name: ExerciseListView::ExerciseListView
// Created: LGY 2012-05-30
// -----------------------------------------------------------------------------
ExerciseListView::ExerciseListView( QWidget* widget, const tools::GeneralConfig& config, const tools::Loader_ABC& fileLoader )
    : Q3ListView( widget )
    , config_    ( config )
    , fileLoader_( fileLoader )
{
    addColumn( "exercise" );
    header()->hide();
    setAllColumnsShowFocus( true );
    setSortColumn( 0 );
    setResizeMode( Q3ListView::LastColumn );
}

// -----------------------------------------------------------------------------
// Name: ExerciseListView::~ExerciseListView
// Created: LGY 2012-05-30
// -----------------------------------------------------------------------------
ExerciseListView::~ExerciseListView()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ExerciseListView::FindExerciseItem
// Created: LGY 2012-05-30k
// -----------------------------------------------------------------------------
Q3ListViewItem* ExerciseListView::FindExerciseItem( const QString& path ) const
{
    for( Q3ListViewItemIterator it = firstChild(); it.current(); ++it )
        if( static_cast< MyListViewItem* >( it.current() )->fullpath_ == path )
            return it.current();
    return 0;
}

// -----------------------------------------------------------------------------
// Name: ExerciseListView::GetExerciseDisplayName
// Created: LGY 2012-05-30
// -----------------------------------------------------------------------------
QString ExerciseListView::GetExerciseDisplayName( const QString& exercise ) const
{
    std::string displayName( exercise.toAscii().constData() );
    try
    {
        const std::string file = config_.GetExerciseFile( exercise.toAscii().constData() );
        if( boost::filesystem::exists( file ) )
        {
            std::auto_ptr< xml::xistream > xis = fileLoader_.LoadFile( file );
            *xis >> xml::start( "exercise" )
                >> xml::optional >> xml::start( "meta" );
        }
    }
    catch( ... )
    {
        // $$$$ SBO 2008-10-07: error in exercise.xml meta, just show directory name
    }
    return displayName.c_str();
}

// -----------------------------------------------------------------------------
// Name: ExerciseListView::AddExerciseEntry
// Created: LGY 2012-05-30
// -----------------------------------------------------------------------------
void ExerciseListView::AddExerciseEntry( const frontend::Exercise_ABC& exercise )
{
    static const QImage directory( "resources/images/selftraining/directory.png" );
    static const QImage mission( "resources/images/selftraining/mission.png" );
    QPixmap pxDir;
    QPixmap pxMiss;

    pxDir.fromImage( directory );
    pxMiss.fromImage( mission );

    qApp->setOverrideCursor( Qt::WaitCursor );
    QStringList path( QStringList::split( '/', exercise.GetName().c_str() ) );
    QStringList complete;
    Q3ListViewItem* parent = 0;
    for( QStringList::iterator it( path.begin() ); it != path.end(); ++it )
    {
        complete.append( *it );
        const QString current( complete.join( "/" ) );
        Q3ListViewItem* item = FindExerciseItem( "/" + current );
        if( !item )
        {
            const std::string fullpath( complete.size() < path.size() ? "/" + current : current );
            if( parent )
                item = new MyListViewItem( parent, fullpath );
            else
                item = new MyListViewItem( this, fullpath );
            item->setText( 0, GetExerciseDisplayName( *it ) );
            QPixmap p;
            p.convertFromImage( directory );
            item->setPixmap( 0, p );
        }
        parent = item;
    }
    if( parent )
    {
        QPixmap p;
        p.convertFromImage( mission );
        parent->setPixmap( 0, p );
        static_cast< gui::ValuedListItem* >( parent )->SetValue( &exercise );
    }
    qApp->restoreOverrideCursor();
}

// -----------------------------------------------------------------------------
// Name: ExerciseListView::DeleteExerciseEntry
// Created: LGY 2012-05-30
// -----------------------------------------------------------------------------
void ExerciseListView::DeleteExerciseEntry( const frontend::Exercise_ABC& exercise )
{
    if( gui::ValuedListItem* item = gui::FindItem( &exercise, firstChild() ) )
    {
        Q3ListViewItem* parent = item->parent();
        delete item;
        if( parent && parent->childCount() == 0 )
            delete parent;
    }
}

// -----------------------------------------------------------------------------
// Name: ExerciseListView::GetItemName
// Created: LGY 2012-05-30
// -----------------------------------------------------------------------------
QString ExerciseListView::GetItemName( Q3ListViewItem* item ) const
{
    return QString( item ? static_cast< MyListViewItem* >( item )->fullpath_ : "" );
}
