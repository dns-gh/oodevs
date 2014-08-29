// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_app_pch.h"
#include "TimelineListView.h"
#include "moc_TimelineListView.cpp"
#include "actions/Action_ABC.h"
#include "actions/ActionsFilter_ABC.h"
#include "actions/ActionTasker.h"
#include "clients_kernel/Controllers.h"
#include "clients_kernel/Entity_ABC.h"
#include "clients_kernel/OrderType.h"
#include "clients_kernel/ActionController.h"
#include "gaming/Model.h"

Q_DECLARE_METATYPE( const kernel::Entity_ABC* )

namespace
{
    class TimelineHeaderView : public QHeaderView
    {
    public:
        explicit TimelineHeaderView( QWidget* parent ) : QHeaderView( Qt::Horizontal, parent )
        {
            setStretchLastSection(true);
        }
        virtual ~TimelineHeaderView() {}

    protected:
        virtual QSize sizeHint() const
        {
            QSize s = QHeaderView::sizeHint();
            s.setHeight( 30 );
            return s;
        }
    };

    class TimelineItemDelegate : public QItemDelegate
    {
    public:
        explicit TimelineItemDelegate( QWidget* parent ) : QItemDelegate( parent ) {}
        virtual ~TimelineItemDelegate() {}

    protected:
        virtual void paint( QPainter* p, const QStyleOptionViewItem& option, const QModelIndex& index) const
        {
            QItemDelegate::paint( p, option, index );
            const QPen oldPen = p->pen();
            p->setPen( QColor( 225, 225, 225 ) );
            const QRect& r = option.rect;
            p->drawLine( r.left(), r.bottom(), r.right(), r.bottom() );
            p->setPen( oldPen );
        }

        virtual QSize sizeHint( const QStyleOptionViewItem& option, const QModelIndex& index) const
        {
            QSize s = QItemDelegate::sizeHint( option, index );
            s.setHeight( 25 );
            return s;
        }
    };
}

// -----------------------------------------------------------------------------
// Name: TimelineListView constructor
// Created: SBO 2008-04-22
// -----------------------------------------------------------------------------
TimelineListView::TimelineListView( QWidget* parent, kernel::Controllers& controllers, const Model& model )
    : QTreeWidget( parent )
    , controllers_( controllers )
    , model_( model )
    , filter_     ( 0 )
    , blockSelect_( false )
{
    setMinimumWidth( 200 );
    setColumnCount( 1 );
    setHeaderLabel( tr( "Units" ) );
    setBackgroundColor( Qt::white );
    setHeader( new TimelineHeaderView( this ) );
    header()->setModel( this->model() );
    setHorizontalScrollBarPolicy( Qt::ScrollBarAlwaysOn ); //--> to have the same height as canvasview
    setVerticalScrollMode( QAbstractItemView::ScrollPerPixel );
    setSortingEnabled( false );
    setRootIsDecorated( false );
    setItemDelegate( new TimelineItemDelegate( this ) );
    magicItem_ = new QTreeWidgetItem();
    magicItem_->setText( 0, tr( "Magic" ) );
    weatherItem_ = new QTreeWidgetItem();
    weatherItem_->setText( 0, tr( "Weather" ) );
    objectItem_ = new QTreeWidgetItem();
    objectItem_->setText( 0, tr( "Objects" ) );
    addTopLevelItem( magicItem_ );
    addTopLevelItem( weatherItem_ );
    addTopLevelItem( objectItem_ );
    magicItem_->setHidden( true );
    weatherItem_->setHidden( true );
    objectItem_->setHidden( true );
    connect( selectionModel(), SIGNAL( selectionChanged( const QItemSelection&, const QItemSelection& ) ), SLOT( OnSelectionChange( const QItemSelection&, const QItemSelection& ) ) );
    connect( verticalScrollBar(), SIGNAL( valueChanged( int ) ), SLOT( OnVScrollbarChanged( int ) ) );
    controllers_.Register( *this );
}

// -----------------------------------------------------------------------------
// Name: TimelineListView destructor
// Created: SBO 2008-04-22
// -----------------------------------------------------------------------------
TimelineListView::~TimelineListView()
{
    controllers_.Unregister( *this );
}

// -----------------------------------------------------------------------------
// Name: TimelineListView::FindItem
// Created: JSR 2012-10-25
// -----------------------------------------------------------------------------
QTreeWidgetItem* TimelineListView::FindItem( const kernel::Entity_ABC* entity ) const
{
    for( int i = 0; i < topLevelItemCount(); ++i )
    {
        QTreeWidgetItem* item = topLevelItem( i );
        if( item && item->data( 0, Qt::UserRole ).isValid() && item->data( 0, Qt::UserRole ).value< const kernel::Entity_ABC* >() == entity )
            return item;
    }
    return 0;
}

// -----------------------------------------------------------------------------
// Name: TimelineListView::FindListItem
// Created: JSR 2011-03-03
// -----------------------------------------------------------------------------
QTreeWidgetItem* TimelineListView::FindListItem( const actions::Action_ABC& action, actions::EActionType& actionType ) const
{
    const actions::ActionTasker* tasker = action.Retrieve< actions::ActionTasker >();
    if( tasker )
    {
        if( const kernel::Entity_ABC* entity = model_.FindEntity( tasker->GetId() ) )
        {
            actionType = actions::eTypeEntity;
            return FindItem( entity );
        }
    }
    if( action.GetType() )
    {
        const std::string& actionTypeName = action.GetType()->GetName();
        if( actionTypeName == "global_weather" || actionTypeName == "local_weather" || actionTypeName == "local_weather_destruction" )
        {
            actionType = actions::eTypeWeather;
            return weatherItem_;
        }
        if( actionTypeName == "create_object" || actionTypeName == "update_object" ||
            actionTypeName == "destroy_object" || actionTypeName == "request_object" )
        {
            actionType = actions::eTypeObjects;
            return objectItem_;
        }
    }
    actionType = actions::eTypeMagic;
    return magicItem_;
}

// -----------------------------------------------------------------------------
// Name: TimelineListView::NotifyCreated
// Created: SBO 2008-04-22
// -----------------------------------------------------------------------------
void TimelineListView::NotifyCreated( const actions::Action_ABC& action )
{
    actions::EActionType actionType;
    QTreeWidgetItem* item = FindListItem( action, actionType );
    switch( actionType )
    {
    case actions::eTypeEntity :
        {
            if( const kernel::Entity_ABC* entity = model_.FindEntity( action.Get< actions::ActionTasker >().GetId() ) ) // cannot be null
            {
                if( !item )
                {
                    item = new QTreeWidgetItem();
                    item->setText( 0, entity->GetName() );
                    item->setData( 0, Qt::UserRole, QVariant::fromValue( entity ) );
                    addTopLevelItem( item );
                }
                // $$$$ _RC_ JSR 2011-03-03: on cache toute la ligne si une action n'est pas autorisée?? a vérifier
                item->setHidden( filter_ && !filter_->Allows( action ) );
                entityActions_[ entity ].push_back( &action );
            }
        }
        break;
    case actions::eTypeWeather :
        {
            item->setHidden( filter_ && !filter_->Allows( action ) );
            weatherActions_.push_back( &action );
        }
        break;
    case actions::eTypeObjects :
        {
            item->setHidden( filter_ && !filter_->Allows( action ) );
            objectsActions_.push_back( &action );
        }
        break;
    case actions::eTypeMagic:
    default:
        {
            item->setHidden( filter_ && !filter_->Allows( action ) );
            magicActions_.push_back( &action );
        }
    }
}

// -----------------------------------------------------------------------------
// Name: TimelineListView::NotifyDeleted
// Created: SBO 2008-04-22
// -----------------------------------------------------------------------------
void TimelineListView::NotifyDeleted( const actions::Action_ABC& action )
{
    actions::EActionType actionType;
    QTreeWidgetItem* item = FindListItem( action, actionType );
    if( !item )
        return;
    T_Actions* actions = 0;
    switch( actionType )
    {
    case actions::eTypeEntity :
    {
        const kernel::Entity_ABC* entity = model_.FindEntity( action.Get< actions::ActionTasker >().GetId() ); // cannot be null;
        actions = &entityActions_[ entity ];
        break;
    }
    case actions::eTypeWeather :
        actions = &weatherActions_;
        break;
    case actions::eTypeObjects :
        actions = &objectsActions_;
        break;
    case actions::eTypeMagic:
    default:
        actions = &magicActions_;
    }
    T_Actions::iterator it = std::find( actions->begin(), actions->end(), &action );
    if( it != actions->end() )
        actions->erase( it );
    if( actions->empty() )
    {
        if( actionType == actions::eTypeEntity )
            delete item;
        else
            item->setHidden( true );
        Update();
    }
}

// -----------------------------------------------------------------------------
// Name: TimelineListView::NotifyDeleted
// Created: SBO 2008-04-22
// -----------------------------------------------------------------------------
void TimelineListView::NotifyDeleted( const kernel::Entity_ABC& entity )
{
    if( QTreeWidgetItem* item = FindItem( &entity ) )
    {
        entityActions_.erase( &entity );
        delete item;
    }
}

// -----------------------------------------------------------------------------
// Name: TimelineListView::NotifySelected
// Created: JSR 2012-12-03
// -----------------------------------------------------------------------------
void TimelineListView::NotifySelected( const kernel::Entity_ABC* entity )
{
    blockSelect_ = true;
    selectionModel()->clearSelection();
    if( QTreeWidgetItem* item = FindItem( entity ) )
    {
        const QModelIndex index = indexFromItem( item );
        selectionModel()->select( index, QItemSelectionModel::Select );
        scrollTo( index );
    }
    blockSelect_ = false;
}

// -----------------------------------------------------------------------------
// Name: TimelineListView::SetContentsPos
// Created: JSR 2012-10-26
// -----------------------------------------------------------------------------
void TimelineListView::SetContentsPos( int /*dx*/, int dy )
{
    bool wasBlocked = blockSignals( true );
    verticalScrollBar()->setValue( dy );
    blockSignals( wasBlocked );
}

// -----------------------------------------------------------------------------
// Name: TimelineListView::OnVScrollbarChanged
// Created: JSR 2012-10-26
// -----------------------------------------------------------------------------
void TimelineListView::OnVScrollbarChanged( int y )
{
    emit ContentsMoving( 0, y );
}

// -----------------------------------------------------------------------------
// Name: TimelineListView::OnSelectionChange
// Created: SBO 2008-04-29
// -----------------------------------------------------------------------------
void TimelineListView::OnSelectionChange( const QItemSelection&, const QItemSelection& )
{
    if( blockSelect_ )
        return;
    QList< QTreeWidgetItem* > items = selectedItems();
    QTreeWidgetItem* item = 0;
    if( items.size() > 0 )
        item = items[ 0 ];
    if( item && item->data( 0, Qt::UserRole ).isValid() )
    {
        kernel::GraphicalEntity_ABC::T_GraphicalEntities list;
        list.push_back( item->data( 0, Qt::UserRole ).value< const kernel::Entity_ABC* >() );
        controllers_.actions_.SetMultipleSelection( list );
    }
}

// -----------------------------------------------------------------------------
// Name: TimelineListView::SetFilter
// Created: SBO 2010-07-19
// -----------------------------------------------------------------------------
void TimelineListView::SetFilter( const actions::ActionsFilter_ABC& filter )
{
    if( filter_ != &filter )
    {
        filter_ = &filter;
        Update();
    }
}

// -----------------------------------------------------------------------------
// Name: TimelineListView::Update
// Created: SBO 2010-07-19
// -----------------------------------------------------------------------------
void TimelineListView::Update()
{
    for( int i = 0; i < topLevelItemCount(); ++i)
    {
        QTreeWidgetItem* item = topLevelItem( i );
        if( item && item->data( 0, Qt::UserRole ).isValid() )
            item->setHidden( !ShouldDisplay( *item->data( 0, Qt::UserRole ).value< const kernel::Entity_ABC* >() ) );
    }
}

// -----------------------------------------------------------------------------
// Name: TimelineListView::ShouldDisplay
// Created: SBO 2010-07-19
// -----------------------------------------------------------------------------
bool TimelineListView::ShouldDisplay( const kernel::Entity_ABC& entity ) const
{
    if( !filter_ )
        return true;
    T_EntityActions::const_iterator actions = entityActions_.find( &entity );
    for( T_Actions::const_iterator it = actions->second.begin(); it != actions->second.end(); ++it )
        if( filter_->Allows( **it ) )
            return true;
    return false;
}
