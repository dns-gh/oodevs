// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#include "clients_gui_pch.h"
#include "ResourceLinksDialog_ABC.h"
#include "moc_ResourceLinksDialog_ABC.cpp"

#include "ResourceNetwork_ABC.h"
#include "RichCheckBox.h"
#include "RichGroupBox.h"
#include "RichWidget.h"
#include "RichSpinBox.h"
#include "RichPushButton.h"
#include "RichWidget.h"
#include "SubObjectName.h"

#include "clients_kernel/Controllers.h"
#include "clients_kernel/Controller.h"
#include "clients_kernel/Hierarchies.h"
#include "clients_kernel/Model_ABC.h"
#include "clients_kernel/Object_ABC.h"
#include "clients_kernel/ObjectType.h"
#include "clients_kernel/Tools.h"
#include "clients_kernel/ResourceNetworkType.h"
#include "clients_kernel/UrbanObject_ABC.h"

using namespace gui;

// -----------------------------------------------------------------------------
// Name: ResourceLinksDialog_ABC constructor
// Created: JSR 2010-08-24
// -----------------------------------------------------------------------------
ResourceLinksDialog_ABC::ResourceLinksDialog_ABC( QMainWindow* parent, const kernel::Model_ABC& model, kernel::Controllers& controllers, const tools::StringResolver< kernel::ResourceNetworkType >& resources )
    : RichDockWidget( controllers, parent, "resource", tools::translate( "gui::ResourceLinksDialog_ABC", "Resource Networks" ) )
    , controllers_( controllers )
    , model_( model )
    , selectedItem_( 0 )
    , blockSlot_( false )
    , resources_( resources )
    , sourceNode_( 0 )
    , id_( 0 )
{
    SubObjectName subObject( this->objectName() );
    Q3VBox* mainLayout = new Q3VBox( this );
    setWidget( mainLayout );
    pMainLayout_ = new Q3VBox( mainLayout );
    pMainLayout_->setSpacing( 5 );
    pMainLayout_->setMargin( 5 );
    Q3VBox* pNodeBox = new Q3VBox( pMainLayout_ );
    dotationList_ = new RichWidget< QListWidget >( "datationList", pNodeBox );
    dotationList_->setMaximumHeight( 60 );
    connect( dotationList_, SIGNAL( currentRowChanged ( int ) ), this, SLOT( Update() ) );
    groupBox_ = new RichGroupBox( "EnabledgroupBox", tools::translate( "gui::ResourceLinksDialog_ABC", "Enabled" ), pNodeBox );
    QVBoxLayout* groupBoxLayout = new QVBoxLayout( groupBox_ );
    groupBox_->setCheckable( true );
    connect( groupBox_, SIGNAL( toggled( bool ) ), this, SLOT( OnActivationChanged( bool ) ) );
    {
        Q3HBox* box = new Q3HBox();
        groupBoxLayout->addWidget( box );
        new QLabel( tools::translate( "gui::ResourceLinksDialog_ABC", "Production:" ), box );
        production_ = new RichSpinBox( "production", box );
        connect( production_, SIGNAL( valueChanged( int ) ), this, SLOT( OnProductionChanged( int ) ) );
    }
    generateProduction_ = new RichPushButton( "generateProduction", tools::translate( "gui::ResourceLinksDialog_ABC", "Automatic production" ) );
    groupBoxLayout->addWidget( generateProduction_ );
    connect( generateProduction_, SIGNAL( clicked() ), this, SLOT( GenerateProduction() ) );
    generateProduction_->hide();
    {
        Q3HBox* box = new Q3HBox();
        groupBoxLayout->addWidget( box );
        new QLabel( tools::translate( "gui::ResourceLinksDialog_ABC", "Consumption:" ), box );
        consumption_  = new RichSpinBox( "consumption", box );
        connect( consumption_, SIGNAL( valueChanged( int ) ), this, SLOT( OnConsumptionChanged( int ) ) );
    }
    critical_ = new RichCheckBox( "critical", tools::translate( "gui::ResourceLinksDialog_ABC", "Vital consumption" ) );
    groupBoxLayout->addWidget( critical_ );
    connect( critical_, SIGNAL( toggled( bool ) ), this, SLOT( OnCriticalChanged( bool ) ) );
    {
        Q3HBox* box = new Q3HBox();
        groupBoxLayout->addWidget( box );
        new QLabel( tools::translate( "gui::ResourceLinksDialog_ABC", "Maximal stock:" ), box );
        maxStock_ = new RichSpinBox( "maxStock", box );
        connect( maxStock_, SIGNAL( valueChanged( int ) ), this, SLOT( OnMaxStockChanged( int ) ) );
    }
    {
        stockBox_ = new Q3HBox();
        groupBoxLayout->addWidget( stockBox_ );
        new QLabel( tools::translate( "gui::ResourceLinksDialog_ABC", "Initial stock:" ), stockBox_ );
        stock_ = new RichSpinBox( "stock", stockBox_ );
        connect( stock_, SIGNAL( valueChanged( int ) ), this, SLOT( OnStockChanged( int ) ) );
        stockBox_->hide();
    }
    table_ = new RichWidget< QTableWidget >( "table" );
    table_->setSelectionMode( QAbstractItemView::NoSelection );
    table_->setColumnCount( 3 );
    groupBoxLayout->addWidget( table_ );
    QStringList headers;
    headers << tools::translate( "gui::ResourceLinksDialog_ABC", "Target" )
            << tools::translate( "gui::ResourceLinksDialog_ABC", "Limited" )
            << tools::translate( "gui::ResourceLinksDialog_ABC", "Capacity" );
    table_->setHorizontalHeaderLabels( headers );
    table_->horizontalHeader()->setResizeMode( 0, QHeaderView::Stretch );
    table_->horizontalHeader()->setResizeMode( 1, QHeaderView::ResizeToContents );
    table_->horizontalHeader()->setResizeMode( 2, QHeaderView::ResizeToContents );
    table_->horizontalHeader()->setStretchLastSection( false );
    groupBox_->setEnabled( false );
    connect( table_, SIGNAL( cellChanged( int, int ) ), SLOT( OnValueChanged() ) );
    Q3HBox* buttonBox = new Q3HBox( pMainLayout_ );
    okButton_ = new RichPushButton( "ok", tools::translate( "gui::ResourceLinksDialog_ABC", "Validate" ), buttonBox );
    okButton_->setDefault( true );
    connect( okButton_, SIGNAL( clicked() ), SLOT( Validate() ) );

    cancelButton_ = new RichPushButton( "cancel", tools::translate( "gui::ResourceLinksDialog_ABC", "Cancel" ), buttonBox );
    cancelButton_->setDefault( true );
    connect( cancelButton_, SIGNAL( clicked() ), SLOT( Cancel() ) );

    controllers_.Update( *this );
    pMainLayout_->hide();
}

// -----------------------------------------------------------------------------
// Name: ResourceLinksDialog_ABC destructor
// Created: JSR 2010-08-24
// -----------------------------------------------------------------------------
ResourceLinksDialog_ABC::~ResourceLinksDialog_ABC()
{
    controllers_.Unregister( *this );
}

// -----------------------------------------------------------------------------
// Name: ResourceLinksDialog_ABC::BeforeMultiSelection
// Created: JSR 2010-09-09
// -----------------------------------------------------------------------------
void ResourceLinksDialog_ABC::BeforeMultiSelection()
{
    selected_.clear();
    selectedItem_ = 0;
    pMainLayout_->hide();
}

// -----------------------------------------------------------------------------
// Name: ResourceLinksDialog_ABC::AfterMultiSelection
// Created: JSR 2010-09-09
// -----------------------------------------------------------------------------
void ResourceLinksDialog_ABC::AfterMultiSelection()
{
    Show();
}

namespace
{
    void AddSubUrbanObjects( const kernel::UrbanObject_ABC* object, std::set< const kernel::UrbanObject_ABC* >& objects )
    {
        objects.insert( object );
        const kernel::Hierarchies* hierarchies = object->Retrieve< kernel::Hierarchies >();
        if( hierarchies )
        {
            auto subIt = hierarchies->CreateSubordinateIterator();
            while( subIt.HasMoreElements() )
                AddSubUrbanObjects( static_cast< const kernel::UrbanObject_ABC* >( &subIt.NextElement() ), objects );
        }
    }
}

// -----------------------------------------------------------------------------
// Name: ResourceLinksDialog_ABC::MultipleSelect
// Created: JSR 2012-05-30
// -----------------------------------------------------------------------------
void ResourceLinksDialog_ABC::MultipleSelect( const std::vector< const kernel::UrbanObject_ABC* >& elements )
{
    std::set< const kernel::UrbanObject_ABC* > selectedBlocks;
    for( auto it = elements.begin(); it != elements.end(); ++it )
        AddSubUrbanObjects( *it, selectedBlocks );
    std::vector< const kernel::UrbanObject_ABC* > selectedBlocksVector;
    std::copy( selectedBlocks.begin(), selectedBlocks.end(), std::back_inserter( selectedBlocksVector ) );
    DoMultipleSelect( selectedBlocksVector );
}

// -----------------------------------------------------------------------------
// Name: ResourceLinksDialog_ABC::MultipleSelect
// Created: JSR 2012-05-30
// -----------------------------------------------------------------------------
void ResourceLinksDialog_ABC::MultipleSelect( const std::vector< const kernel::Object_ABC* >& elements )
{
    DoMultipleSelect( elements );
}

// -----------------------------------------------------------------------------
// Name: ResourceLinksDialog_ABC::DoMultipleSelect
// Created: JSR 2012-05-30
// -----------------------------------------------------------------------------
template< class T >
void ResourceLinksDialog_ABC::DoMultipleSelect( const std::vector< const T* >& elements )
{
    id_ = ( elements.size() == 1 ? elements.front()->GetId() : 0 );
    for( std::vector< const T* >::const_iterator it = elements.begin(); it != elements.end(); ++it )
    {
        kernel::Entity_ABC* obj = const_cast< kernel::Entity_ABC* >( static_cast< const kernel::Entity_ABC* >( *it ) );
        if( obj->Retrieve< ResourceNetwork_ABC >() )
            selected_.push_back( obj );
    }
}

// -----------------------------------------------------------------------------
// Name: ResourceLinksDialog_ABC::Update
// Created: JSR 2010-08-24
// -----------------------------------------------------------------------------
void ResourceLinksDialog_ABC::Update()
{
    SubObjectName subObject( this->objectName() );
    if( selected_.size() != 1 )
        return;
    okButton_->setVisible( !IsReadOnly() );
    cancelButton_->setVisible( !IsReadOnly() );
    QListWidgetItem* item = dotationList_->currentItem();
    if( !item )
    {
        if( dotationList_->count() > 0 )
            dotationList_->setCurrentItem( selectedItem_, QItemSelectionModel::ClearAndSelect );
        else
            selectedItem_ = 0;
        return;
    }
    selectedItem_ = item;
    std::string resource = item->text().toStdString();
    ResourceNode& node = resourceNodes_[ resource ];
    groupBox_->setChecked( node.isEnabled_ );
    production_->setValue( node.production_ );
    consumption_->setValue( node.consumption_ );
    critical_->setChecked( node.critical_ );
    maxStock_->setValue( node.maxStock_ );
    stock_->setValue( node.stock_ );

    blockSlot_ = true;
    table_->setRowCount( static_cast< int >( node.links_.size() ) );
    for( unsigned int j = 0; j < node.links_.size(); ++j )
    {
        SubObjectName subObject( selected_.front()->Get< ResourceNetwork_ABC >().GetLinkName( resource, j ) );
        bool limited = node.links_[ j ].capacity_ != -1;
        {
            QTableWidgetItem* item1 = new QTableWidgetItem( selected_.front()->Get< ResourceNetwork_ABC >().GetLinkName( resource, j ) );
            table_->setItem( j, 0, item1 );
            item1->setFlags( item1->flags() ^ Qt::ItemIsEditable );
        }
        {
            table_->setCellWidget( j, 1, 0 );
            table_->setItem( j, 1, new QTableWidgetItem() );
            RichCheckBox* limitedBox = new RichCheckBox( "limitedBox" );
            limitedBox->setEnabled( controllers_.GetCurrentMode() != eModes_Replay );
            limitedBox->setCheckState( limited ? Qt::Checked : Qt::Unchecked );
            table_->setCellWidget( j, 1, limitedBox );
            connect( limitedBox, SIGNAL( stateChanged( int ) ), SLOT( OnValueChanged() ) );
        }
        {
            table_->setCellWidget( j, 2, 0 );
            table_->setItem( j, 2, new QTableWidgetItem() );
            RichSpinBox* capacity = new RichSpinBox( "capacity" );
            capacity->setEnabled( limited && controllers_.GetCurrentMode() != eModes_Replay );
            capacity->setRange( 0, std::numeric_limits< int >::max() );
            capacity->setValue( limited ? node.links_[ j ].capacity_ : 0 );
            table_->setCellWidget( j, 2, capacity );
            connect( capacity, SIGNAL( valueChanged( int ) ), SLOT( OnValueChanged() ) );
        }
    }
    blockSlot_ = false;
}

// -----------------------------------------------------------------------------
// Name: ResourceLinksDialog_ABC::OnActivationChanged
// Created: JSR 2010-09-02
// -----------------------------------------------------------------------------
void ResourceLinksDialog_ABC::OnActivationChanged( bool on )
{
    if( GetCurrentMode() != eModes_Replay )
    {
        if( dotationList_->currentItem() )
            resourceNodes_[ dotationList_->currentItem()->text().toStdString() ].isEnabled_ = on;
    }
    else
        if( resourceNodes_[ dotationList_->currentItem()->text().toStdString() ].isEnabled_ != on )
            groupBox_->setChecked( !on );
}

// -----------------------------------------------------------------------------
// Name: ResourceLinksDialog_ABC::OnProductionChanged
// Created: JSR 2010-09-02
// -----------------------------------------------------------------------------
void ResourceLinksDialog_ABC::OnProductionChanged( int value )
{
    if( dotationList_->currentItem() )
        resourceNodes_[ dotationList_->currentItem()->text().toStdString() ].production_ = value;
}

// -----------------------------------------------------------------------------
// Name: ResourceLinksDialog_ABC::OnConsumptionChanged
// Created: JSR 2010-09-02
// -----------------------------------------------------------------------------
void ResourceLinksDialog_ABC::OnConsumptionChanged( int value )
{
    if( dotationList_->currentItem() )
        resourceNodes_[ dotationList_->currentItem()->text().toStdString() ].consumption_ = value;
}

// -----------------------------------------------------------------------------
// Name: ResourceLinksDialog_ABC::OnCriticalChanged
// Created: JSR 2010-09-02
// -----------------------------------------------------------------------------
void ResourceLinksDialog_ABC::OnCriticalChanged( bool on )
{
    if( dotationList_->currentItem() )
        resourceNodes_[ dotationList_->currentItem()->text().toStdString() ].critical_ = on;
}

// -----------------------------------------------------------------------------
// Name: ResourceLinksDialog_ABC::OnMaxStockChanged
// Created: JSR 2010-09-08
// -----------------------------------------------------------------------------
void ResourceLinksDialog_ABC::OnMaxStockChanged( int value )
{
    if( dotationList_->currentItem() )
        resourceNodes_[ dotationList_->currentItem()->text().toStdString() ].maxStock_ = value;
}

// -----------------------------------------------------------------------------
// Name: ResourceLinksDialog_ABC::OnStockChanged
// Created: JSR 2010-09-02
// -----------------------------------------------------------------------------
void ResourceLinksDialog_ABC::OnStockChanged( int value )
{
    if( dotationList_->currentItem() )
        resourceNodes_[ dotationList_->currentItem()->text().toStdString() ].stock_ = value;
}

// -----------------------------------------------------------------------------
// Name: ResourceLinksDialog_ABC::OnValueChanged
// Created: JSR 2010-08-30
// -----------------------------------------------------------------------------
void ResourceLinksDialog_ABC::OnValueChanged()
{
    if( blockSlot_ )
        return;
    if( dotationList_->currentItem() )
    {
        blockSlot_ = true;
        std::string resource = dotationList_->currentItem()->text().toStdString();
        for( int j = 0; j < table_->rowCount(); ++j )
        {
            bool enable = ( static_cast< RichCheckBox* >( table_->cellWidget( j, 1 ) )->checkState() == Qt::Checked );
            table_->cellWidget( j, 2 )->setEnabled( enable && controllers_.GetCurrentMode() != eModes_Replay );
            if( enable )
                resourceNodes_[ resource ].links_[ j ].capacity_ = static_cast< RichSpinBox* >( table_->cellWidget( j, 2 ) )->value();
            else
                resourceNodes_[ resource ].links_[ j ].capacity_ = -1;
        }
        blockSlot_ = false;
    }
}

// -----------------------------------------------------------------------------
// Name: ResourceLinksDialog_ABC::Validate
// Created: JSR 2010-08-24
// -----------------------------------------------------------------------------
void ResourceLinksDialog_ABC::Validate()
{
    if( selected_.size() != 1 )
        return;
    if( dotationList_->currentItem() )
    {
        // in case spin boxes have not been validated
        std::string resource = dotationList_->currentItem()->text().toStdString();
        resourceNodes_[ resource ].production_ = production_->value();
        resourceNodes_[ resource ].consumption_ = consumption_->value();
        resourceNodes_[ resource ].maxStock_ = maxStock_->value();
        resourceNodes_[ resource ].stock_ = stock_->value();
        DoValidate();
        controllers_.controller_.Update( selected_.front()->Get< ResourceNetwork_ABC >() );
    }
}

// -----------------------------------------------------------------------------
// Name: ResourceLinksDialog_ABC::Cancel
// Created: NPT 2013-03-28
// -----------------------------------------------------------------------------
void ResourceLinksDialog_ABC::Cancel()
{
    Show();
}

// -----------------------------------------------------------------------------
// Name: ResourceLinksDialog_ABC::NotifyDeleted
// Created: JSR 2010-09-09
// -----------------------------------------------------------------------------
void ResourceLinksDialog_ABC::NotifyDeleted( const kernel::Entity_ABC& element )
{
    std::vector< kernel::Entity_ABC* >::iterator it = std::find( selected_.begin(), selected_.end(), &element );
    if( it != selected_.end() )
    {
        pMainLayout_->hide();
        selected_.erase( it );
    }
}

// -----------------------------------------------------------------------------
// Name: ResourceLinksDialog_ABC::NotifyUpdated
// Created: NPT 2013-03-08
// -----------------------------------------------------------------------------
void ResourceLinksDialog_ABC::NotifyUpdated( const gui::ResourceNetwork_ABC& /*element*/ )
{
    if( controllers_.GetCurrentMode() == eModes_Replay )
        Show();
}

// -----------------------------------------------------------------------------
// Name: ResourceLinksDialog_ABC::NotifyContextMenu
// Created: JSR 2011-02-24
// -----------------------------------------------------------------------------
void ResourceLinksDialog_ABC::NotifyContextMenu( const kernel::Object_ABC& object, kernel::ContextMenu& menu )
{
    DoNotifyContextMenu( object, menu );
}

// -----------------------------------------------------------------------------
// Name: ResourceLinksDialog_ABC::NotifyContextMenu
// Created: JSR 2012-06-01
// -----------------------------------------------------------------------------
void ResourceLinksDialog_ABC::NotifyContextMenu( const kernel::UrbanObject_ABC& object, kernel::ContextMenu& menu )
{
    DoNotifyContextMenu( object, menu );
}

// -----------------------------------------------------------------------------
// Name: ResourceLinksDialog_ABC::DoNotifyContextMenu
// Created: JSR 2012-06-01
// -----------------------------------------------------------------------------
void ResourceLinksDialog_ABC::DoNotifyContextMenu( const kernel::Entity_ABC& entity, kernel::ContextMenu& menu )
{
    if( resources_.Count() == 0 )
        return;
    const ResourceNetwork_ABC* node = entity.Retrieve< ResourceNetwork_ABC >();
    if( !node || selected_.empty() || GetCurrentMode() == eModes_Replay )
        return;
    sourceNode_ = const_cast< kernel::Entity_ABC* >( &entity );
    kernel::ContextMenu* subMenu = menu.SubMenu( "Resource", tr( "Resource networks" ) );
    tools::Iterator< const kernel::ResourceNetworkType& > it = resources_.CreateIterator();
    int resourceId = 0;
    while( it.HasMoreElements() )
    {
        const kernel::ResourceNetworkType& resource = it.NextElement();
        kernel::ContextMenu* resourceMenu = new kernel::ContextMenu( subMenu );
        subMenu->insertItem( QString::fromStdString( resource.GetName() ), resourceMenu );
        if( selected_.size() == 1 && selected_.front() == &entity )
        {
            if( !node->FindResourceNode( resource.GetName() ) )
                resourceMenu->insertItem( tr( "Create node" ), this , SLOT( OnCreateNode( int ) ), 0, resourceId );
            else
                resourceMenu->insertItem( tr( "Remove node" ), this , SLOT( OnRemoveNode( int ) ), 0, resourceId );
        }
        else
        {
            if( std::find( selected_.begin(), selected_.end(), &entity ) == selected_.end() )
                resourceMenu->insertItem( tr( "Add/Remove link" ), this , SLOT( OnChangeLink( int ) ), 0, resourceId );
            else
            {
                bool displayCreate = false;
                bool displayRemove = false;
                for( auto it = selected_.begin(); it != selected_.end(); ++it )
                {
                    kernel::Entity_ABC* selected = *it;
                    ResourceNetwork_ABC& resourceNetwork = selected->Get< ResourceNetwork_ABC >();
                    if( resourceNetwork.FindResourceNode( resource.GetName() ) )
                    {
                        displayRemove = true;
                        if( displayCreate )
                            break;
                    }
                    else
                    {
                        displayCreate = true;
                        if( displayRemove )
                            break;
                    }
                }
                if( displayCreate )
                    resourceMenu->insertItem( tr( "Create nodes" ), this , SLOT( OnCreateNode( int ) ), 0, resourceId );
                if( displayRemove )
                    resourceMenu->insertItem( tr( "Remove nodes" ), this , SLOT( OnRemoveNode( int ) ), 0, resourceId );
            }
        }
        ++resourceId;
    }
}

// -----------------------------------------------------------------------------
// Name: ResourceLinksDialog_ABC::SetReadOnly
// Created: NPT 2013-02-28
// -----------------------------------------------------------------------------
void ResourceLinksDialog_ABC::SetReadOnly( bool readOnly ) const
{
    production_->setReadOnly( readOnly );
    consumption_->setReadOnly( readOnly );
    critical_->setEnabled( !readOnly );
    maxStock_->setReadOnly( readOnly );
    stock_->setReadOnly( readOnly );
    table_->setEditTriggers( readOnly? QAbstractItemView::NoEditTriggers : QAbstractItemView::CurrentChanged );
    generateProduction_->setEnabled( !readOnly );
    okButton_->setEnabled( !readOnly );
    cancelButton_->setEnabled( !readOnly );
}

// -----------------------------------------------------------------------------
// Name: ResourceLinksDialog_ABC::RefreshListNeeded
// Created: JSR 2013-05-31
// -----------------------------------------------------------------------------
bool ResourceLinksDialog_ABC::RefreshListNeeded() const
{
    if( static_cast< int >( resourceNodes_.size() ) != dotationList_->count() )
        return true;
    unsigned int row  = 0;
    for( auto it = resourceNodes_.begin(); it != resourceNodes_.end(); ++it, ++row )
        if( dotationList_->item( row )->text().toStdString() != it->first )
            return true;
    return false;
}

// -----------------------------------------------------------------------------
// Name: ResourceLinksDialog_ABC::Show
// Created: JSR 2010-08-24
// -----------------------------------------------------------------------------
void ResourceLinksDialog_ABC::Show()
{
    if( selected_.size() != 1 )
        return;
    resourceNodes_ = selected_.front()->Get< ResourceNetwork_ABC >().GetResourceNodes();
    if( RefreshListNeeded() )
    {
        bool wasBlocked = dotationList_->blockSignals( true );
        dotationList_->clear();
        selectedItem_ = 0;
        dotationList_->blockSignals( wasBlocked );
        for( auto it = resourceNodes_.begin(); it != resourceNodes_.end(); ++it )
            if( it == resourceNodes_.begin() )
            {
                selectedItem_ = new QListWidgetItem( it->second.resource_.c_str(), dotationList_ );
                dotationList_->setCurrentItem( selectedItem_ );
            }
            else
                new QListWidgetItem( it->second.resource_.c_str(), dotationList_ );
    }

    if( dotationList_->count() > 0 )
        groupBox_->setEnabled( true );
    else
    {
        groupBox_->setEnabled( false );
        production_->setValue( 0 );
        consumption_->setValue( 0 );
        critical_->setChecked( false );
        maxStock_->setValue( 0 );
        stock_->setValue( 0 );
        table_->clearContents();
        table_->setRowCount( 0 );
    }
    if( selectedItem_ )
    {
        std::string resource = selectedItem_->text().toStdString();
        groupBox_->setChecked( resourceNodes_[ resource ].isEnabled_ );
    }
    Update();
    pMainLayout_->show();
}

namespace
{
    template< class T >
    bool HasIncomingLinks( unsigned int dstId, const std::string& resource, const tools::Resolver_ABC< T >& resolver )
    {
        auto it = resolver.CreateIterator();
        while( it.HasMoreElements() )
        {
            const T& object = it.NextElement();
            if( const ResourceNetwork_ABC* srcNetwork = object.Retrieve< ResourceNetwork_ABC >() )
            {
                if( const ResourceNode* srcNode = srcNetwork->FindResourceNode( resource ) )
                {
                    for( auto itLink = srcNode->links_.begin(); itLink != srcNode->links_.end(); ++itLink )
                        if( itLink->id_ == dstId )
                            return true;
                }
            }
        }
        return false;
    }

    bool HasResourceLink( const ResourceNetwork_ABC& network, unsigned int dstId, const kernel::Model_ABC& model, const std::string& resource )
    {
        // outgoing links
        const ResourceNode* node = network.FindResourceNode( resource );
        if( node && !node->links_.empty() )
            return true;
        // incoming links
        return HasIncomingLinks( dstId, resource, model.GetUrbanObjectResolver() )
            || HasIncomingLinks( dstId, resource, model.GetObjectResolver() );
    }
}

// -----------------------------------------------------------------------------
// Name: ResourceLinksDialog_ABC::OnChangeLink
// Created: JSR 2011-03-01
// -----------------------------------------------------------------------------
void ResourceLinksDialog_ABC::OnChangeLink( int resourceId )
{
    if( sourceNode_ == 0 )
        return;
    auto* node = sourceNode_->Retrieve< ResourceNetwork_ABC >();
    if( !node )
        return;
    typedef std::map< std::string, ResourceNode > T_ResourceNodes;
    tools::Iterator< const kernel::ResourceNetworkType& > it = resources_.CreateIterator();
    int index = 0;
    while( it.HasMoreElements() )
    {
        const kernel::ResourceNetworkType& resource = it.NextElement();
        if( index++ == resourceId )
        {
            ResourceNode& sourceNode = node->FindOrCreateResourceNode( resource.GetName() );
            for( std::vector< kernel::Entity_ABC* >::iterator it = selected_.begin(); it != selected_.end(); ++it )
            {
                kernel::Entity_ABC& dest = **it;
                auto destNode = dest.Retrieve< ResourceNetwork_ABC >();
                if( destNode == 0 ||  destNode == node )
                    continue;
                destNode->FindOrCreateResourceNode( resource.GetName() );
                bool destUrban = ( dynamic_cast< kernel::UrbanObject_ABC* >( &dest ) != 0 );
                unsigned long destId = dest.GetId();
                std::vector< ResourceLink >::iterator itLink;
                for( itLink = sourceNode.links_.begin(); itLink != sourceNode.links_.end(); ++itLink )
                    if( itLink->urban_ == destUrban && itLink->id_ == destId )
                        break;
                if( itLink == sourceNode.links_.end() )
                    sourceNode.links_.push_back( ResourceLink( destUrban, destId ) );
                else
                {
                    sourceNode.links_.erase( itLink );
                    if( !HasResourceLink( *destNode, dest.GetId(), model_, resource.GetName() ) )
                    {
                        destNode->RemoveNode( resource.GetName() );
                        controllers_.controller_.Update( *destNode );
                    }
                }
            }
            if( !HasResourceLink( *node, sourceNode_->GetId(), model_, resource.GetName() ) )
                node->RemoveNode( resource.GetName() );
            controllers_.controller_.Update( *node );
            break;
        }
    }
    Show();
    DoValidate( sourceNode_ );
    sourceNode_ = 0;
}

// -----------------------------------------------------------------------------
// Name: ResourceLinksDialog_ABC::OnCreateNode
// Created: JSR 2011-11-28
// -----------------------------------------------------------------------------
void ResourceLinksDialog_ABC::OnCreateNode( int resourceId )
{
    tools::Iterator< const kernel::ResourceNetworkType& > it = resources_.CreateIterator();
    int index = 0;
    while( it.HasMoreElements() ) // $$$$ LDC RC WTF? Why not resources_.Get or Find( resourceId) ????
    {
        const kernel::ResourceNetworkType& resource = it.NextElement();
        if( index++ == resourceId )
        {
            for( auto selectedIt = selected_.begin(); selectedIt != selected_.end(); ++selectedIt )
            {
                ResourceNetwork_ABC& resourceNetwork =  ( *selectedIt )->Get< ResourceNetwork_ABC >();
                resourceNetwork.FindOrCreateResourceNode( resource.GetName(), resource.GetDefaultProduction() );
                controllers_.controller_.Update( resourceNetwork );
            }
            break;
        }
    }
    Show();
    DoValidate();
}

// -----------------------------------------------------------------------------
// Name: ResourceLinksDialog_ABC::OnRemoveNode
// Created: LDC 2012-08-08
// -----------------------------------------------------------------------------
void ResourceLinksDialog_ABC::OnRemoveNode( int resourceId )
{
    tools::Iterator< const kernel::ResourceNetworkType& > it = resources_.CreateIterator();
    int index = 0;
    while( it.HasMoreElements() ) // $$$$ LDC RC WTF? Why not resources_.Get or Find( resourceId ) ????
    {
        const kernel::ResourceNetworkType& resource = it.NextElement();
        if( index++ == resourceId )
        {
            for( auto selectedIt = selected_.begin(); selectedIt != selected_.end(); ++selectedIt )
            {
                kernel::Entity_ABC* selected = *selectedIt;
                ResourceNetwork_ABC& resourceNetwork = selected->Get< ResourceNetwork_ABC >();
                bool isUrban = ( dynamic_cast< kernel::UrbanObject_ABC* >( selected ) != 0 );
                resourceNetwork.RemoveNode( resource.GetName() );
                controllers_.controller_.Update( resourceNetwork );
                ResourceLinkDeletion deletion;
                deletion.resource_ = resource.GetName();
                deletion.isUrban_ = isUrban;
                deletion.id_ = selected->GetId();
                controllers_.controller_.Update( deletion );
            }
            break;
        }
    }
    Show();
    DoValidate();
}

// -----------------------------------------------------------------------------
// Name: ResourceLinksDialog_ABC::GenerateProduction
// Created: JSR 2011-09-20
// -----------------------------------------------------------------------------
void ResourceLinksDialog_ABC::GenerateProduction()
{
    if( selected_.size() != 1 )
        return;
    if( dotationList_->currentItem() )
        if( DoGenerateProduction() )
        {
            DoValidate();
            controllers_.controller_.Update( selected_.front()->Get< ResourceNetwork_ABC >() );
            Update();
        }
}

// -----------------------------------------------------------------------------
// Name: ResourceLinksDialog_ABC::IsDataUpdateEnabled
// Created: NPT 2013-04-11
// -----------------------------------------------------------------------------
bool ResourceLinksDialog_ABC::IsDataUpdateEnabled()
{
    if( !selectedItem_ )
        return true;
    std::string resource = selectedItem_->text().toStdString();
    ResourceNode& node = resourceNodes_[ resource ];
    return ( production_->value() == ( int ) node.production_ 
                 && consumption_->value() == ( int ) node.consumption_ 
                 && maxStock_->value() == ( int ) node.maxStock_ ) 
                 || controllers_.GetCurrentMode() == eModes_Replay;
}
