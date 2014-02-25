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
#include "Application.h"
#include "frontend/Exercise_ABC.h"
#include "tools/GeneralConfig.h"
#include "tools/Loader_ABC.h"
#include <xeumeuleu/xml.hpp>

Q_DECLARE_METATYPE( const frontend::Exercise_ABC* )

#define FullpathRole ( Qt::UserRole + 1 )
#define ExerciseRole ( Qt::UserRole + 2 )

namespace
{
    class CustomSortFilterProxyModel : public QSortFilterProxyModel
    {
    public:
        CustomSortFilterProxyModel( QStandardItemModel& model )
            : QSortFilterProxyModel( &model )
            , model_( model )
        {
            // NOTHING
        }
    protected:
        virtual bool lessThan( const QModelIndex& left, const QModelIndex& right ) const
        {
           QStandardItem* itemLeft = model_.itemFromIndex( left );
           QStandardItem* itemRight = model_.itemFromIndex( right );
           if( !itemLeft->data( ExerciseRole ).isValid() && itemRight->data( ExerciseRole ).isValid() )
                return false;
           if( itemLeft->data( ExerciseRole ).isValid() && !itemRight->data( ExerciseRole ).isValid() )
                return true;
            return itemLeft->text().localeAwareCompare( itemRight->text() ) > 0;
        }
    public:
        const QStandardItemModel& model_;
    };
}

// -----------------------------------------------------------------------------
// Name: ExerciseListView::ExerciseListView
// Created: LGY 2012-05-30
// -----------------------------------------------------------------------------
ExerciseListView::ExerciseListView( Application& app, const tools::GeneralConfig& config, const tools::Loader_ABC& fileLoader )
    : app_( app )
    , config_( config )
    , fileLoader_( fileLoader )
    , proxy_( new CustomSortFilterProxyModel( model_ ) )
{
    proxy_->setSourceModel( &model_ );
    setModel( proxy_ );
    setSortingEnabled( true );
    proxy_->setDynamicSortFilter( true );
    setHeaderHidden( true );
    setRootIsDecorated( false );
    setEditTriggers( 0 );
    setFont( QFont( "Calibri", 12, QFont::Bold ) );
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
// Name: ExerciseListView::GetSelectedExerciseItem
// Created: NPT 2012-09-26
// -----------------------------------------------------------------------------
const QStandardItem* ExerciseListView::GetSelectedExerciseItem() const
{
    const QModelIndexList selected = selectedIndexes();
    if( selected.empty() )
        return 0;
    const QModelIndex index = selected.front();
    if( index.isValid() )
        return model_.itemFromIndex( proxy_->mapToSource( index ) );
    return 0;
}

// -----------------------------------------------------------------------------
// Name: ExerciseListView::GetSelectedExercise
// Created: NPT 2012-09-26
// -----------------------------------------------------------------------------
const frontend::Exercise_ABC* ExerciseListView::GetSelectedExercise() const
{
    if( const QStandardItem* item = GetSelectedExerciseItem() ) 
    {
        QVariant v = item->data( ExerciseRole );
        if( v.isValid() )
            return v.value< const frontend::Exercise_ABC* >();
    }
    return 0;
}

// -----------------------------------------------------------------------------
// Name: ExerciseListView::Clear
// Created: NPT 2012-09-26
// -----------------------------------------------------------------------------
void ExerciseListView::Clear()
{
    model_.clear();
}

// -----------------------------------------------------------------------------
// Name: ExerciseListView::Exists
// Created: NPT 2012-09-26
// -----------------------------------------------------------------------------
bool ExerciseListView::Exists( const tools::Path& exercise ) const
{
    return model_.findItems( exercise.ToUTF8().c_str() ).size() > 0;
}

// -----------------------------------------------------------------------------
// Name: ExerciseListView::FindExerciseItem
// Created: LGY 2012-05-30k
// -----------------------------------------------------------------------------
QStandardItem* ExerciseListView::FindExerciseItem( const QString& path, QStandardItem* parent ) const
{
    if( !parent )
        parent = model_.invisibleRootItem();
    for( int row = 0; row < parent->rowCount(); ++row )
    {
        QStandardItem* item = parent->child( row );
        if( item->data( FullpathRole ).toString() == path )
            return item;
        if( QStandardItem* found = FindExerciseItem( path, item ) )
            return found;
    }
    return 0;
}

// -----------------------------------------------------------------------------
// Name: ExerciseListView::AddExerciseEntry
// Created: LGY 2012-05-30
// -----------------------------------------------------------------------------
void ExerciseListView::AddExerciseEntry( const frontend::Exercise_ABC& exercise )
{
    static const QPixmap directory( "resources/images/selftraining/directory.png" );
    static const QPixmap mission( "resources/images/selftraining/mission.png" );

    app_.SetWaitingCursor();
    tools::Path exerciseName = exercise.GetName();
    QStringList path( QString( exerciseName.ToUTF8().c_str() ).split( '\\') );
    QStringList complete;
    QStandardItem* parent = 0;
    for( auto it = path.constBegin(); it != path.constEnd(); ++it )
    {
        complete.append( *it );
        const QString current( complete.join( "\\" ) );
        QStandardItem* item = FindExerciseItem( "\\" + current );
        if( !item )
        {
            const std::string fullpath( complete.size() < path.size() ? "\\" + current.toStdString() : current.toStdString() );
            item = new QStandardItem( directory, *it );
            item->setData( QString( fullpath.c_str() ), FullpathRole );
            item->setFlags( Qt::ItemIsEnabled );
            if( parent )
                parent->setChild( parent->rowCount(), item );
            else
                model_.appendRow( item );
        }
        parent = item;
    }
    if( parent )
    {
        parent->setIcon( mission );
        parent->setData( QVariant::fromValue( &exercise ), ExerciseRole );
        parent->setFlags( Qt::ItemIsEnabled | Qt::ItemIsSelectable );
    }
}

// -----------------------------------------------------------------------------
// Name: ExerciseListView::FindExerciseItem
// Created: NPT 2012-09-26
// -----------------------------------------------------------------------------
QStandardItem* ExerciseListView::FindExerciseItem( const frontend::Exercise_ABC& exercise, QStandardItem* parent ) const
{
    if( !parent )
        parent = model_.invisibleRootItem();
    for( int row = 0; row < parent->rowCount(); ++row )
    {
        QStandardItem* item = parent->child( row );
        if( item->data( ExerciseRole ).isValid() )
            if( item->data( ExerciseRole ).value< const frontend::Exercise_ABC* >() == &exercise )
                return item;
        if( QStandardItem* child = FindExerciseItem( exercise, item ) )
            return child;
    }
    return 0;
}

namespace
{
    void DeleteExerciseTree( QStandardItem* item, QStandardItem* invisibleRootItem )
    {
        QStandardItem* parent = item->parent();
        if( !parent )
            parent = invisibleRootItem;
        parent->removeRow( item->row() );
        if( parent->rowCount() == 0 && parent != invisibleRootItem )
            DeleteExerciseTree( parent, invisibleRootItem );
    }
}

// -----------------------------------------------------------------------------
// Name: ExerciseListView::DeleteExerciseEntry
// Created: LGY 2012-05-30
// -----------------------------------------------------------------------------
void ExerciseListView::DeleteExerciseEntry( const frontend::Exercise_ABC& exercise )
{
    if( QStandardItem* item = FindExerciseItem( exercise ) )
        DeleteExerciseTree( item, model_.invisibleRootItem() );
}

// -----------------------------------------------------------------------------
// Name: ExerciseListView::GetExerciseName
// Created: LGY 2012-05-30
// -----------------------------------------------------------------------------
tools::Path ExerciseListView::GetExerciseName( const QModelIndex& index ) const
{
    if( index.isValid() )
    {
        QStandardItem* item = model_.itemFromIndex( proxy_->mapToSource( index ) );
        if( item && item->data( FullpathRole ).isValid() && item->data( ExerciseRole ).isValid() )
            return tools::Path::FromUnicode( item->data( FullpathRole ).toString().toStdWString() );
    }
    return tools::Path();
}
