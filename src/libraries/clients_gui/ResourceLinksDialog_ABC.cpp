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
#include "RichSpinBox.h"
#include "tools.h"

#include "clients_kernel/Controllers.h"
#include "clients_kernel/Controller.h"
#include "clients_kernel/Object_ABC.h"
#include "clients_kernel/ObjectType.h"
#include "clients_kernel/ResourceNetworkType.h"
#include "clients_kernel/UrbanObject_ABC.h"

using namespace gui;
using namespace kernel;

// -----------------------------------------------------------------------------
// Name: ResourceLinksDialog_ABC constructor
// Created: JSR 2010-08-24
// -----------------------------------------------------------------------------
ResourceLinksDialog_ABC::ResourceLinksDialog_ABC( QMainWindow* parent, Controllers& controllers, const tools::StringResolver< ResourceNetworkType >& resources )
    : RichDockWidget   ( controllers, parent, "resource", tools::translate( "gui::ResourceLinksDialog_ABC", "Resource Networks" ) )
    , controllers_     ( controllers )
    , selectedItem_    ( 0 )
    , blockSlot_       ( false )
    , resources_       ( resources )
    , sourceNode_      ( 0 )
    , id_              ( 0 )
{
    Q3VBox* mainLayout = new Q3VBox( this );
    setWidget( mainLayout );
    pMainLayout_ = new Q3VBox( mainLayout );
    pMainLayout_->setSpacing( 5 );
    pMainLayout_->setMargin( 5 );
    Q3VBox* pNodeBox = new Q3VBox( pMainLayout_ );
    dotationList_ = new QListWidget( pNodeBox );
    dotationList_->setMaximumHeight( 60 );
    connect( dotationList_, SIGNAL( currentRowChanged ( int ) ), this, SLOT( Update() ) );
    groupBox_ = new Q3GroupBox( 1, Qt::Horizontal, tools::translate( "gui::ResourceLinksDialog_ABC", "Enabled" ), pNodeBox );
    groupBox_->setCheckable( true );
    connect( groupBox_, SIGNAL( toggled( bool ) ), this, SLOT( OnActivationChanged( bool ) ) );
    {
        Q3HBox* box = new Q3HBox( groupBox_ );
        new QLabel( tools::translate( "gui::ResourceLinksDialog_ABC", "Production:" ), box );
        production_ = new RichSpinBox( box );
        connect( production_, SIGNAL( valueChanged( int ) ), this, SLOT( OnProductionChanged( int ) ) );
    }
    generateProduction_ = new QPushButton( tools::translate( "gui::ResourceLinksDialog_ABC", "Automatic production" ), groupBox_ );
    connect( generateProduction_, SIGNAL( clicked() ), this, SLOT( GenerateProduction() ) );
    generateProduction_->hide();
    {
        Q3HBox* box = new Q3HBox( groupBox_ );
        new QLabel( tools::translate( "gui::ResourceLinksDialog_ABC", "Consumption:" ), box );
        consumption_  = new RichSpinBox( box );
        connect( consumption_, SIGNAL( valueChanged( int ) ), this, SLOT( OnConsumptionChanged( int ) ) );
    }
    critical_ = new QCheckBox( tools::translate( "gui::ResourceLinksDialog_ABC", "Vital consumption" ), groupBox_ );
    connect( critical_, SIGNAL( toggled( bool ) ), this, SLOT( OnCriticalChanged( bool ) ) );
    {
        Q3HBox* box = new Q3HBox( groupBox_ );
        new QLabel( tools::translate( "gui::ResourceLinksDialog_ABC", "Maximal stock:" ), box );
        maxStock_ = new RichSpinBox( box );
        connect( maxStock_, SIGNAL( valueChanged( int ) ), this, SLOT( OnMaxStockChanged( int ) ) );
    }
    {
        stockBox_ = new Q3HBox( groupBox_ );
        new QLabel( tools::translate( "gui::ResourceLinksDialog_ABC", "Initial stock:" ), stockBox_ );
        stock_ = new RichSpinBox( stockBox_ );
        connect( stock_, SIGNAL( valueChanged( int ) ), this, SLOT( OnStockChanged( int ) ) );
        stockBox_->hide();
    }
    table_ = new QTableWidget( groupBox_ );
    table_->setSelectionMode( QAbstractItemView::NoSelection );
    table_->setColumnCount( 3 );
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
    QPushButton* okBtn = new QPushButton( tools::translate( "gui::ResourceLinksDialog_ABC", "Validate" ), pMainLayout_ );
    okBtn->setDefault( true );
    connect( okBtn, SIGNAL( clicked() ), SLOT( Validate() ) );
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
// Name: ResourceLinksDialog_ABC::BeforeSelection
// Created: JSR 2010-09-09
// -----------------------------------------------------------------------------
void ResourceLinksDialog_ABC::BeforeSelection()
{
    selected_.clear();
    selectedItem_ = 0;
    pMainLayout_->hide();
}

// -----------------------------------------------------------------------------
// Name: ResourceLinksDialog_ABC::AfterSelection
// Created: JSR 2010-09-09
// -----------------------------------------------------------------------------
void ResourceLinksDialog_ABC::AfterSelection()
{
    Show();
}

// -----------------------------------------------------------------------------
// Name: ResourceLinksDialog_ABC::MultipleSelect
// Created: JSR 2012-05-30
// -----------------------------------------------------------------------------
void ResourceLinksDialog_ABC::MultipleSelect( const std::vector< const kernel::UrbanObject_ABC* >& elements )
{
    DoMultipleSelect( elements );
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
    if( selected_.size() != 1 )
        return;
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
    ResourceNetwork_ABC::ResourceNode& node = resourceNodes_[ resource ];
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
        bool limited = node.links_[ j ].capacity_ != -1;
        {
            QTableWidgetItem* item1 = new QTableWidgetItem( selected_.front()->Get< ResourceNetwork_ABC >().GetLinkName( resource, j ) );
            table_->setItem( j, 0, item1 );
            item1->setFlags( item1->flags() ^ Qt::ItemIsEditable );
        }
        {
            table_->setItem( j, 1, new QTableWidgetItem() );
            QCheckBox* limitedBox = new QCheckBox;
            limitedBox->setCheckState( limited ? Qt::Checked : Qt::Unchecked );
            table_->setCellWidget( j, 1, limitedBox );
            connect( limitedBox, SIGNAL( stateChanged( int ) ), SLOT( OnValueChanged() ) );
        }
        {
            table_->setItem( j, 2, new QTableWidgetItem() );
            QSpinBox* capacity = new QSpinBox();
            capacity->setEnabled( limited );
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
    if( dotationList_->currentItem() )
        resourceNodes_[ dotationList_->currentItem()->text().toStdString() ].isEnabled_ = on;
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
            bool enable = ( static_cast< QCheckBox* >( table_->cellWidget( j, 1 ) )->checkState() == Qt::Checked );
            table_->cellWidget( j, 2 )->setEnabled( enable );
            if( enable )
                resourceNodes_[ resource ].links_[ j ].capacity_ = static_cast< QSpinBox* >( table_->cellWidget( j, 2 ) )->value();
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
// Name: ResourceLinksDialog_ABC::NotifyDeleted
// Created: JSR 2010-09-09
// -----------------------------------------------------------------------------
void ResourceLinksDialog_ABC::NotifyDeleted( const Entity_ABC& element )
{
    std::vector< kernel::Entity_ABC* >::iterator it = std::find( selected_.begin(), selected_.end(), &element );
    if( it != selected_.end() )
    {
        pMainLayout_->hide();
        selected_.erase( it );
    }
}

// -----------------------------------------------------------------------------
// Name: ResourceLinksDialog_ABC::NotifyContextMenu
// Created: JSR 2011-02-24
// -----------------------------------------------------------------------------
void ResourceLinksDialog_ABC::NotifyContextMenu( const Object_ABC& object, ContextMenu& menu )
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
    if( !node || selected_.empty() )
        return;
    sourceNode_ = const_cast< kernel::Entity_ABC* >( &entity );
    ContextMenu* subMenu = menu.SubMenu( "Resource", tr( "Resource networks" ) );
    tools::Iterator< const ResourceNetworkType& > it = resources_.CreateIterator();
    int resourceId = 0;
    while( it.HasMoreElements() )
    {
        const ResourceNetworkType& resource = it.NextElement();
        if( selected_.size() == 1 && selected_.front() == &entity )
        {
            if( !node->FindResourceNode( resource.GetName() ) )
            {
                ContextMenu* resourceMenu = new ContextMenu( subMenu );
                subMenu->insertItem( QString::fromUtf8( resource.GetName().c_str() ), resourceMenu );
                resourceMenu->insertItem( tr( "Create node" ), this , SLOT( OnCreateNode( int ) ), 0, resourceId );
            }
            else
            {
                ContextMenu* resourceMenu = new ContextMenu( subMenu );
                subMenu->insertItem( resource.GetName().c_str(), resourceMenu );
                resourceMenu->insertItem( tr( "Remove node" ), this , SLOT( OnRemoveNode( int ) ), 0, resourceId );
            }
            ++resourceId;
        }
        else
        {
            ContextMenu* resourceMenu = new ContextMenu( subMenu );
            subMenu->insertItem( resource.GetName().c_str(), resourceMenu );
            resourceMenu->insertItem( tr( "Add/Remove link" ), this , SLOT( OnChangeLink( int ) ), 0, resourceId++ );
        }
    }
}

// -----------------------------------------------------------------------------
// Name: ResourceLinksDialog_ABC::Show
// Created: JSR 2010-08-24
// -----------------------------------------------------------------------------
void ResourceLinksDialog_ABC::Show()
{
    dotationList_->clear();
    if( selected_.size() != 1 )
        return;
    resourceNodes_ = selected_.front()->Get< ResourceNetwork_ABC >().GetResourceNodes();
    for( auto it = resourceNodes_.begin(); it != resourceNodes_.end(); ++it )
        if( it == resourceNodes_.begin() )
        {
            selectedItem_ = new QListWidgetItem( it->second.resource_.c_str(), dotationList_ );
            dotationList_->setCurrentItem( selectedItem_ );
        }
        else
            new QListWidgetItem( it->second.resource_.c_str(), dotationList_ );

    if( dotationList_->count() > 0 )
        groupBox_->setEnabled( true );
    else
    {
        groupBox_->setEnabled( false );
        groupBox_->setChecked( true );
        production_->setValue( 0 );
        consumption_->setValue( 0 );
        critical_->setChecked( false );
        maxStock_->setValue( 0 );
        stock_->setValue( 0 );
        table_->clearContents();
        table_->setRowCount( 0 );
    }
    Update();
    pMainLayout_->show();
}

// -----------------------------------------------------------------------------
// Name: ResourceLinksDialog_ABC::OnChangeLink
// Created: JSR 2011-03-01
// -----------------------------------------------------------------------------
void ResourceLinksDialog_ABC::OnChangeLink( int resourceId )
{
    if( sourceNode_ == 0 )
        return;
    auto* node = sourceNode_->Retrieve< gui::ResourceNetwork_ABC >();
    if( !node )
        return;
    typedef ResourceNetwork_ABC::ResourceNode ResourceNode;
    typedef ResourceNetwork_ABC::ResourceLink ResourceLink;
    typedef ResourceNetwork_ABC::T_ResourceNodes T_ResourceNodes;
    tools::Iterator< const ResourceNetworkType& > it = resources_.CreateIterator();
    int index = 0;
    while( it.HasMoreElements() )
    {
        const ResourceNetworkType& resource = it.NextElement();
        if( index++ == resourceId )
        {
            ResourceNode& sourceNode = node->FindOrCreateResourceNode( resource.GetName() );
            for( std::vector< kernel::Entity_ABC* >::iterator it = selected_.begin(); it != selected_.end(); ++it )
            {
                kernel::Entity_ABC& dest = **it;
                if( dest.Retrieve< ResourceNetwork_ABC >() == node )
                    continue;
                dest.Get< gui::ResourceNetwork_ABC >().FindOrCreateResourceNode( resource.GetName() );
                bool destUrban = ( dynamic_cast< kernel::UrbanObject_ABC* >( &dest ) != 0 );
                unsigned long destId = dest.GetId();
                gui::ResourceNetwork_ABC::T_ResourceLinks::iterator itLink;
                for( itLink = sourceNode.links_.begin(); itLink != sourceNode.links_.end(); ++itLink )
                    if( itLink->urban_ == destUrban && itLink->id_ == destId )
                        break;
                if( itLink == sourceNode.links_.end() )
                    sourceNode.links_.push_back( ResourceLink( destUrban, destId ) );
                else
                    sourceNode.links_.erase( itLink );
            }
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
    if( selected_.size() != 1 )
        return;
    tools::Iterator< const ResourceNetworkType& > it = resources_.CreateIterator();
    int index = 0;
    ResourceNetwork_ABC& resourceNetwork = selected_.front()->Get< ResourceNetwork_ABC >();
    while( it.HasMoreElements() ) // $$$$ LDC RC WTF? Why not resources_.Get or Find( resourceId) ????
    {
        const ResourceNetworkType& resource = it.NextElement();
        if( index++ == resourceId )
        {
            resourceNetwork.FindOrCreateResourceNode( resource.GetName(), resource.GetDefaultProduction() );
            controllers_.controller_.Update( resourceNetwork );
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
    if( selected_.size() != 1 )
        return;
    tools::Iterator< const ResourceNetworkType& > it = resources_.CreateIterator();
    int index = 0;
    kernel::Entity_ABC* selected = selected_.front();
    ResourceNetwork_ABC& resourceNetwork = selected->Get< ResourceNetwork_ABC >();
    while( it.HasMoreElements() ) // $$$$ LDC RC WTF? Why not resources_.Get or Find( resourceId ) ????
    {
        const ResourceNetworkType& resource = it.NextElement();
        if( index++ == resourceId )
        {
            bool isUrban = ( dynamic_cast< kernel::UrbanObject_ABC* >( selected ) != 0 );
            resourceNetwork.RemoveNode( resource.GetName() );
            controllers_.controller_.Update( resourceNetwork );
            ResourceNetwork_ABC::Deletion deletion;
            deletion.resource_ = resource.GetName();
            deletion.isUrban_ = isUrban;
            deletion.id_ = selected->GetId();
            controllers_.controller_.Update( deletion );
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
