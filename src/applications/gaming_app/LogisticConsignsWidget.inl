// -----------------------------------------------------------------------------
// Name: LogisticConsignsWidget constructor
// Created: SBO 2007-02-19
// -----------------------------------------------------------------------------
template< typename Consign, typename Extension >
LogisticConsignsWidget< Consign, Extension >::LogisticConsignsWidget( QWidget* parent, kernel::Controllers& controllers
    , kernel::DisplayExtractor_ABC& extractor )
    : LogisticConsignsWidget_ABC( parent, extractor )
    , controllers_( controllers )
    , selected_( controllers )
    , currentItem_( 0 )
    , needUpdating_( true )
{
    controllers_.Register( *this );
}

// -----------------------------------------------------------------------------
// Name: LogisticConsignsWidget destructor
// Created: SBO 2007-02-19
// -----------------------------------------------------------------------------
template< typename Consign, typename Extension >
LogisticConsignsWidget< Consign, Extension >::~LogisticConsignsWidget()
{
    controllers_.Unregister( *this );
}

// -----------------------------------------------------------------------------
// Name: LogisticConsignsWidget::showEvent
// Created: SBO 2007-02-19
// -----------------------------------------------------------------------------
template< typename Consign, typename Extension >
void LogisticConsignsWidget< Consign, Extension >::showEvent( QShowEvent* event )
{
    const kernel::Entity_ABC* entity = selected_;
    selected_ = 0;
    NotifySelected( entity );
    QWidget::showEvent( event );
}

// -----------------------------------------------------------------------------
// Name: LogisticConsignsWidget::FindTreeWidgetItem
// Created: MMC 2012-10-29
// -----------------------------------------------------------------------------
template< typename Consign, typename Extension >
QTreeWidgetItem* LogisticConsignsWidget< Consign, Extension >::FindTreeWidgetItem( const Consign& consign, QTreeWidgetItem* rootItem )
{
    if( !rootItem )
        return 0;
    int count = rootItem->childCount();
    for( int i=0; i< count; ++i )
    {
        QTreeWidgetItem* curItem = rootItem->child( i );
        if( !curItem )
            continue;
        if( curItem->data( 0, Qt::UserRole ).toInt() == reinterpret_cast< int >( &consign ) )
            return curItem;
    }
    return 0;
}

// -----------------------------------------------------------------------------
// Name: LogisticConsignsWidget::AddEntityConsignsToSet
// Created: MMC 2013-01-17
// -----------------------------------------------------------------------------
template< typename Consign, typename Extension >
void LogisticConsignsWidget< Consign, Extension >::AddEntityConsignsToSet( kernel::SafePointer< kernel::Entity_ABC > entity, std::set< const Consign* >& requestedConsigns, std::set< const Consign* >& handledConsigns )
{
    if( !entity )
        return;
    const Extension* pConsigns = entity->Retrieve< Extension >();
    if( pConsigns )
    {
        requestedConsigns.insert( pConsigns->requested_.begin(), pConsigns->requested_.end() );
        handledConsigns.insert( pConsigns->handled_.begin(), pConsigns->handled_.end() );
    }
}

// -----------------------------------------------------------------------------
// Name: LogisticConsignsWidget::NotifyUpdated
// Created: SBO 2007-02-19
// -----------------------------------------------------------------------------
template< typename Consign, typename Extension >
void LogisticConsignsWidget< Consign, Extension >::NotifyUpdated( const Extension& consigns )
{
    if( !selected_ )
        return;
    if( logistic_helpers::HasRetrieveEntityAndSubordinatesUpToBaseLog< Extension >( *selected_, &consigns ) )
        needUpdating_ = true;
}

// -----------------------------------------------------------------------------
// Name: LogisticConsignsWidget::NotifyUpdated
// Created: LDC 2013-05-21
// -----------------------------------------------------------------------------
template< typename Consign, typename Extension >
void LogisticConsignsWidget< Consign, Extension >::NotifyUpdated( const Simulation::sEndTick& )
{
    if( needUpdating_ )
    {
        UpdateConsigns();
        needUpdating_ = false;
    }
}

// -----------------------------------------------------------------------------
// Name: LogisticConsignsWidget::UpdateConsigns
// Created: SBO 2007-02-19
// -----------------------------------------------------------------------------
template< typename Consign, typename Extension >
void LogisticConsignsWidget< Consign, Extension >::UpdateConsigns()
{
    if( !selected_ )
        return;
    AddLogisticConsignsToSetFunctor< Consign, Extension > merge;
    logistic_helpers::VisitEntityAndSubordinatesUpToBaseLog( *selected_
                                                           , boost::bind( &AddLogisticConsignsToSetFunctor< Consign, Extension >::Add
                                                           , &merge, _1 ) );
    DisplayConsigns( merge.requestedConsigns_, *pConsignTreeView_->invisibleRootItem() );
    DisplayConsigns( merge.handledConsigns_, *pConsignHandledTreeView_->invisibleRootItem() );
}

// -----------------------------------------------------------------------------
// Name: LogisticConsignsWidget::SetTreeWidgetItemData
// Created: MMC 2012-10-29
// -----------------------------------------------------------------------------
template< typename Consign, typename Extension >
void LogisticConsignsWidget< Consign, Extension >::DisplayTitle( const QString& key, const QString& value )
{
    if( currentItem_ )
        SetTreeWidgetItemData( *currentItem_, key, value );
}

// -----------------------------------------------------------------------------
// Name: LogisticConsignsWidget::SetTreeWidgetItemData
// Created: MMC 2012-10-29
// -----------------------------------------------------------------------------
template< typename Consign, typename Extension >
void LogisticConsignsWidget< Consign, Extension >::DisplayItem( const QString& key, const QString& value )
{
    if( currentItem_ )
        AddTreeWidgetItem( *currentItem_, key, value );
}

// -----------------------------------------------------------------------------
// Name: LogisticConsignsWidget::SetTreeWidgetItemData
// Created: MMC 2012-10-29
// -----------------------------------------------------------------------------
template< typename Consign, typename Extension >
void LogisticConsignsWidget< Consign, Extension >::DisplaySubItemValues( const QString& key, const QString& subKey,
                                                                         const QMap< QString, T_OrderedValues >& subValues )
{
    if( !currentItem_ )
        return;
    QTreeWidgetItem* subItem = AddTreeWidgetItem( *currentItem_, key, "" );
    if( !subItem )
        return;
    QTreeWidgetItem* subsubItem = AddTreeWidgetItem( *subItem, subKey, "" );
    if( !subsubItem )
        return;
    SetTreeWidgetItemData( *subsubItem, subKey, "" );
    for( QMap< QString, T_OrderedValues >::const_iterator it = subValues.begin(); it != subValues.end(); ++it )
    {
        QTreeWidgetItem* curItem = AddTreeWidgetItem( *subsubItem, it.key(), "" );
        const T_OrderedValues& quantities = it.value();
        for( std::size_t i = 0; i < quantities.size(); ++i )
            AddTreeWidgetItem( *curItem, quantities[ i ].first, extractor_.GetDisplayName( quantities[ i ].second ) );
    }
}

namespace
{
    void DeleteChilds( QTreeWidgetItem& item )
    {
        while( item.childCount() > 0 )
        {
            DeleteChilds( *item.child( 0 ) );
            delete item.takeChild( 0 );
        }
    }
}

// -----------------------------------------------------------------------------
// Name: LogisticConsignsWidget::UpdateConsign
// Created: LDC 2013-06-27
// -----------------------------------------------------------------------------
template< typename Consign, typename Extension >
void LogisticConsignsWidget< Consign, Extension >::UpdateConsign( const Consign& consign, QTreeWidgetItem* consignItem )
{
    if( consignItem )
    {
        DeleteChilds( *consignItem );
        DisplayConsign( consign, consignItem );
    }
}

// -----------------------------------------------------------------------------
// Name: LogisticConsignsWidget::NotifyUpdated
// Created: SBO 2007-02-19
// -----------------------------------------------------------------------------
template< typename Consign, typename Extension >
void LogisticConsignsWidget< Consign, Extension >::NotifyUpdated( const Consign& consign )
{
    UpdateConsign( consign, FindTreeWidgetItem( consign, pConsignTreeView_->invisibleRootItem() ) );
    UpdateConsign( consign, FindTreeWidgetItem( consign, pConsignHandledTreeView_->invisibleRootItem() ) );
}

// -----------------------------------------------------------------------------
// Name: LogisticConsignsWidget::NotifySelected
// Created: SBO 2007-02-19
// -----------------------------------------------------------------------------
template< typename Consign, typename Extension >
void LogisticConsignsWidget< Consign, Extension >::NotifySelected( const kernel::Entity_ABC* entity )
{
    selected_ = entity;
    if( entity )
        UpdateConsigns();
}

// -----------------------------------------------------------------------------
// Name: LogisticConsignsWidget::Display
// Created: SBO 2007-02-19
// -----------------------------------------------------------------------------
template< typename Consign, typename Extension >
void LogisticConsignsWidget< Consign, Extension >::DisplayConsign( const Consign& consign, QTreeWidgetItem* pCurrentItem )
{
    if( pCurrentItem )
    {
        pCurrentItem->setTextColor( 0, pCurrentItem->textColor( 1 ) );
        pCurrentItem->setData( 0, Qt::UserRole, QVariant( reinterpret_cast< int >( &consign ) ) );
    }
    currentItem_ = pCurrentItem;
    consign.Display( *this, extractor_ );
    currentItem_ = 0;
}

// -----------------------------------------------------------------------------
// Name: LogisticConsignsWidget::DisplayConsigns
// Created: SBO 2007-03-30
// -----------------------------------------------------------------------------
template< typename Consign, typename Extension >
void LogisticConsignsWidget< Consign, Extension >::DisplayConsigns( const std::set< const Consign* >& consigns, QTreeWidgetItem& rootItem )
{
    DeleteChilds( rootItem );
    for( std::set< const Consign* >::const_iterator it = consigns.begin(); it != consigns.end(); ++it )
    {
        const Consign* pConsign = *it;
        if( !pConsign )
            continue;
        QTreeWidgetItem* pNewItem = new QTreeWidgetItem( rootItem );
        DisplayConsign( *pConsign, pNewItem );
        rootItem.addChild( pNewItem );
    }
}
