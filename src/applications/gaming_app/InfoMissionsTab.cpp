// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_app_pch.h"
#include "InfoMissionsTab.h"
#include "moc_InfoMissionsTab.cpp"
#include "icons.h"
#include "actions/Action_ABC.h"
#include "actions/ActionTiming.h"
#include "actions/ActionError.h"
#include "actions/Parameter_ABC.h"
#include "clients_gui/DisplayExtractor.h"
#include "clients_gui/LinkItemDelegate.h"
#include "clients_kernel/Tools.h"
#include "clients_kernel/Controllers.h"
#include "gaming/MissionParameters.h"

namespace
{
    class CustomSortFilterProxyModel : public QSortFilterProxyModel
    {
    public:
        CustomSortFilterProxyModel( QObject* parent )
            : QSortFilterProxyModel( parent )
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
            if( left.isValid() && right.isValid() )
            {
                if( left.parent().isValid() )
                {
                    QString txt1 = sourceModel()->data( left ).toString();
                    QString txt2 = sourceModel()->data( right ).toString();
                    return txt1.localeAwareCompare( txt2 ) > 0;
                }
                return true;
            }
            return false;
        }
    };
}

// -----------------------------------------------------------------------------
// Name: InfoMissionsTab constructor
// Created: SBO 2007-04-18
// -----------------------------------------------------------------------------
InfoMissionsTab::InfoMissionsTab( QTabWidget* parent, kernel::Controllers& controllers, gui::DisplayExtractor& extractor )
    : QTreeView( parent )
    , controllers_( controllers )
    , extractor_( extractor )
    , selected_( controllers )
    , parent_( parent )
    , proxyFilter_( new CustomSortFilterProxyModel( parent ) )
    , error_( QImage( "resources/images/gaming/mission_error.png" ) )
{
    gui::LinkItemDelegate* delegate = new gui::LinkItemDelegate( this );
    setItemDelegateForColumn( 1, delegate );
    setModel( proxyFilter_ );
    setMouseTracking( true );
    setRootIsDecorated( false );
    setEditTriggers( 0 );
    setAlternatingRowColors( true );
    setAllColumnsShowFocus( true );
    proxyFilter_->setSourceModel( &missionModel_ );
    proxyFilter_->setDynamicSortFilter( true );
    setSortingEnabled( true );

    //Add alternative palette
    QPalette p( palette() );
    p.setColor( QPalette::Base, Qt::white );
    p.setColor( QPalette::AlternateBase, QColor( 240, 240, 240 ) );
    setPalette( p );

    //configure the model
    missionModel_.setColumnCount( 5 );
    setHeaderHidden( true );
    header()->setResizeMode( 0, QHeaderView::ResizeToContents );
    header()->setResizeMode( 1, QHeaderView::ResizeToContents );
    header()->setResizeMode( 2, QHeaderView::ResizeToContents );
    header()->setResizeMode( 3, QHeaderView::ResizeToContents );
    header()->setResizeMode( 4, QHeaderView::ResizeToContents );

    connect( delegate, SIGNAL( LinkClicked( const QString&, const QModelIndex& ) ), SLOT( OnLinkClicked( const QString&, const QModelIndex& ) ) );

    controllers_.Register( *this );
}

// -----------------------------------------------------------------------------
// Name: InfoMissionsTab destructor
// Created: SBO 2007-04-18
// -----------------------------------------------------------------------------
InfoMissionsTab::~InfoMissionsTab()
{
    controllers_.Unregister( *this );
}

// -----------------------------------------------------------------------------
// Name: InfoMissionsTab::OnLinkClicked
// Created: JSR 2012-10-23
// -----------------------------------------------------------------------------
void InfoMissionsTab::OnLinkClicked( const QString& url, const QModelIndex& index )
{
    selectionModel()->setCurrentIndex( index, QItemSelectionModel::ClearAndSelect | QItemSelectionModel::Rows );
    extractor_.NotifyLinkClicked( url );
}

// -----------------------------------------------------------------------------
// Name: InfoMissionsTab::NotifySelected
// Created: SBO 2007-04-18
// -----------------------------------------------------------------------------
void InfoMissionsTab::NotifySelected( const kernel::Entity_ABC* entity )
{
    if( selected_ != entity )
    {
        selected_ = entity;
        const MissionParameters* extension = entity ? entity->Retrieve< MissionParameters >() : 0;
        if( extension )
            NotifyUpdated( *extension );
        else
            missionModel_.removeRows( 0, missionModel_.rowCount() );
    }
}

namespace
{
    void ConfigureItem( QStandardItem* item, Qt::Alignment alignment, bool error, const QString& text )
    {
        item->setText( text );
        item->setTextAlignment(alignment );
        if( error )
            item->setData( Qt::red, Qt::TextColorRole );
    }
}

// -----------------------------------------------------------------------------
// Name: InfoMissionsTab::NotifyUpdated
// Created: SBO 2007-04-18
// -----------------------------------------------------------------------------
void InfoMissionsTab::NotifyUpdated( const MissionParameters& extension )
{
    if( ! ShouldUpdate( extension ) )
        return;
    const int count = extension.Count();
    while( missionModel_.rowCount() > count )
        missionModel_.removeRow( count );
    while( missionModel_.rowCount() < count )
    {
        QList< QStandardItem* > items;
        items << new QStandardItem() << new QStandardItem()
              << new QStandardItem() << new QStandardItem();
        missionModel_.appendRow( items );
    }

    int row = 0;
    tools::Iterator< const actions::Action_ABC& > iterator = extension.CreateIterator();
    while( iterator.HasMoreElements() )
    {
        const actions::Action_ABC& action = iterator.NextElement();
        QStandardItem* nameItem = missionModel_.item( row );
        nameItem->setIcon( action.IsValid() ? MAKE_PIXMAP( mission ) : QPixmap::fromImage( error_ ) );
        ConfigureItem( nameItem, Qt::AlignLeft | Qt::AlignVCenter, !action.IsValid(), action.GetName() );

        if( const actions::ActionTiming* timing = action.Retrieve< actions::ActionTiming >() )
            ConfigureItem( missionModel_.item( row, 2 ), Qt::AlignCenter, !action.IsValid(), timing->GetTime().toString() );

        if( const actions::ActionError* error = action.Retrieve< actions::ActionError >() )
            ConfigureItem( missionModel_.item( row, 3 ), Qt::AlignRight| Qt::AlignVCenter, !action.IsValid(), error->GetError().c_str() );

        RecursiveDisplay( action, missionModel_.item( row++ ) );
    }
}

// -----------------------------------------------------------------------------
// Name: InfoMissionsTab::RecursiveDisplay
// Created: JSR 2012-10-23
// -----------------------------------------------------------------------------
template< typename T >
void InfoMissionsTab::RecursiveDisplay( const T& element, QStandardItem* item )
{
    const int count = element.Count();
    while( item->rowCount() > count )
        item->removeRow( count );
    while( item->rowCount() < count )
    {
        QList< QStandardItem* > items;
        items << new QStandardItem() << new QStandardItem() << new QStandardItem() << new QStandardItem();
        item->appendRow( items );
    }
    tools::Iterator< const actions::Parameter_ABC& > it = element.CreateIterator();
    int row = 0;
    while( it.HasMoreElements() )
    {
        DisplayParameter( it.NextElement(), item->child( row, 0 ), item->child( row, 1 ) );
        ++row;
    }
}

// -----------------------------------------------------------------------------
// Name: InfoMissionsTab::DisplayParameter
// Created: JSR 2012-10-23
// -----------------------------------------------------------------------------
void InfoMissionsTab::DisplayParameter( const actions::Parameter_ABC& param, QStandardItem* item1, QStandardItem* item2 )
{
    item1->setIcon( MAKE_PIXMAP( parameter2 ) );
    item1->setText( param.GetName() );
    item2->setText( param.GetDisplayName( extractor_ ) );
    RecursiveDisplay( param, item1 );
}

// -----------------------------------------------------------------------------
// Name: InfoMissionsTab::ShouldUpdate
// Created: SBO 2007-04-18
// -----------------------------------------------------------------------------
bool InfoMissionsTab::ShouldUpdate( const MissionParameters& extension ) const
{
    return isVisible() && selected_ && selected_->Retrieve< MissionParameters >() == &extension;
}

// -----------------------------------------------------------------------------
// Name: InfoMissionsTab::showEvent
// Created: SBO 2007-04-18
// -----------------------------------------------------------------------------
void InfoMissionsTab::showEvent( QShowEvent* event )
{
    const kernel::Entity_ABC* selected = selected_;
    selected_ = 0;
    NotifySelected( selected );
    QTreeView::showEvent( event );
}
