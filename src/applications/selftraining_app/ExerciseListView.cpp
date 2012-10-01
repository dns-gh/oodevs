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
#include "tools/GeneralConfig.h"
#include "tools/Loader_ABC.h"
#include <boost/filesystem.hpp>
#include <xeumeuleu/xml.hpp>

namespace bfs = boost::filesystem;

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
        ~CustomSortFilterProxyModel()
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
           else if( itemLeft->data( ExerciseRole ).isValid() && !itemRight->data( ExerciseRole ).isValid() )
                return true;
           else
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
ExerciseListView::ExerciseListView( QWidget* widget, const tools::GeneralConfig& config, const tools::Loader_ABC& fileLoader )
    : QTreeView( widget )
    , config_    ( config )
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
    const QModelIndex index = selectionModel()->currentIndex();
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
bool ExerciseListView::Exists( const QString& exercise ) const
{
    return model_.findItems( exercise ).size() > 0;
}

// -----------------------------------------------------------------------------
// Name: ExerciseListView::FindExerciseItem
// Created: LGY 2012-05-30k
// -----------------------------------------------------------------------------
QStandardItem* ExerciseListView::FindExerciseItem( const QString& path, QStandardItem* parent /*= 0*/ ) const
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
                >> xml::optional >> xml::start( "meta" )
                >> xml::optional >> xml::content( "name", displayName );
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
    static const QPixmap directory( "resources/images/selftraining/directory.png" );
    static const QPixmap mission( "resources/images/selftraining/mission.png" );

    qApp->setOverrideCursor( Qt::WaitCursor );
    QStringList path( QString( exercise.GetName().c_str() ).split( '/') );
    QStringList complete;
    QStandardItem* parent = 0;
    for( QStringList::iterator it( path.begin() ); it != path.end(); ++it )
    {
        complete.append( *it );
        const QString current( complete.join( "/" ) );
        QStandardItem* item = FindExerciseItem( "/" + current );
        if( !item )
        {
            const std::string fullpath( complete.size() < path.size() ? "/" + current.toStdString() : current.toStdString() );
            item = new QStandardItem( directory, GetExerciseDisplayName( *it ) );
            item->setData( QString( fullpath.c_str() ), FullpathRole );
            if( parent )
                parent->setChild( parent->rowCount(), item );
            else
                model_.invisibleRootItem()->setChild( model_.rowCount(), item );
        }
        parent = item;
    }
    if( parent )
    {
        parent->setIcon( mission );
        parent->setData( QVariant::fromValue( &exercise ), ExerciseRole );
    }
    qApp->restoreOverrideCursor();
}

// -----------------------------------------------------------------------------
// Name: ExerciseListView::FindExerciseItem
// Created: NPT 2012-09-26
// -----------------------------------------------------------------------------
QStandardItem* ExerciseListView::FindExerciseItem( const frontend::Exercise_ABC& exercise, QStandardItem* parent /*= 0*/ ) const
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
QString ExerciseListView::GetExerciseName( const QModelIndex& index ) const
{
    if( index.isValid() )
    {
        QStandardItem* item = model_.itemFromIndex( proxy_->mapToSource( index ) );
        if( item && item->data( FullpathRole ).isValid() && item->data( ExerciseRole ).isValid() )
            return item->data( FullpathRole ).toString();
    }
    return QString();
}
