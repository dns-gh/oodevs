// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

/* TRANSLATOR gui::ResourceLinksDialog_ABC */

#include "clients_gui_pch.h"
#include "ResourceLinksDialog_ABC.h"
#include "moc_ResourceLinksDialog_ABC.cpp"
#include "SpinTableItem.h"
#include "TerrainObjectProxy.h"
#include "tools.h"
#include "clients_kernel/Controllers.h"
#include "clients_kernel/Object_ABC.h"
#include "clients_kernel/ResourceNetwork_ABC.h"

using namespace gui;
using namespace kernel;

// -----------------------------------------------------------------------------
// Name: ResourceLinksDialog_ABC constructor
// Created: JSR 2010-08-24
// -----------------------------------------------------------------------------
ResourceLinksDialog_ABC::ResourceLinksDialog_ABC( QMainWindow* parent, Controllers& controllers )
    : QDockWindow      ( parent, "resource" )
    , controllers_     ( controllers )
    , selected_        ( 0 )
    , selectedItem_    ( 0 )
{
    setResizeEnabled( true );
    setCloseMode( QDockWindow::Always );
    setCaption( tools::translate( "gui::ResourceLinksDialog_ABC", "Resource Networks" ) );
    QVBox* mainLayout = new QVBox( this );
    setWidget( mainLayout );
    pMainLayout_ = new QVBox( mainLayout );
    pMainLayout_->setSpacing( 5 );
    pMainLayout_->setMargin( 5 );
    QVBox* pNodeBox = new QVBox( pMainLayout_ );
    dotationList_ = new QListBox( pNodeBox );
    dotationList_->setMaximumHeight( 60 );
    connect( dotationList_, SIGNAL( selectionChanged() ), this, SLOT( Update() ) );
    groupBox_ = new QGroupBox( 1, Qt::Horizontal, tools::translate( "gui::ResourceLinksDialog_ABC", "Enabled" ), pNodeBox );
    groupBox_->setCheckable( true );
    connect( groupBox_, SIGNAL( toggled( bool ) ), this, SLOT( OnActivationChanged( bool ) ) );
    {
        QHBox* box = new QHBox( groupBox_ );
        new QLabel( tools::translate( "gui::ResourceLinksDialog_ABC", "Production:" ), box );
        production_ = new QSpinBox( 0, std::numeric_limits< int >::max(), 1, box );
        connect( production_, SIGNAL( valueChanged( int ) ), this, SLOT( OnProductionChanged( int ) ) );
    }
    {
        QHBox* box = new QHBox( groupBox_ );
        new QLabel( tools::translate( "gui::ResourceLinksDialog_ABC", "Consumption:" ), box );
        consumption_  = new QSpinBox( 0, std::numeric_limits< int >::max(), 1, box );
        connect( consumption_, SIGNAL( valueChanged( int ) ), this, SLOT( OnConsumptionChanged( int ) ) );
    }
    critical_ = new QCheckBox( tools::translate( "gui::ResourceLinksDialog_ABC", "Vital consumption" ), groupBox_ );
    connect( critical_, SIGNAL( toggled( bool ) ), this, SLOT( OnCriticalChanged( bool ) ) );
    {
        QHBox* box = new QHBox( groupBox_ );
        new QLabel( tools::translate( "gui::ResourceLinksDialog_ABC", "Maximal stock:" ), box );
        maxStock_ = new QSpinBox( 0, std::numeric_limits< int >::max(), 1, box );
        connect( maxStock_, SIGNAL( valueChanged( int ) ), this, SLOT( OnMaxStockChanged( int ) ) );
    }
    {
        stockBox_ = new QHBox( groupBox_ );
        new QLabel( tools::translate( "gui::ResourceLinksDialog_ABC", "Initial stock:" ), stockBox_ );
        stock_ = new QSpinBox( 0, std::numeric_limits< int >::max(), 1, stockBox_ );
        connect( stock_, SIGNAL( valueChanged( int ) ), this, SLOT( OnStockChanged( int ) ) );
        stockBox_->hide();
    }
    table_ = new QTable( groupBox_ );
    table_->setSelectionMode( QTable::NoSelection );
    table_->setNumCols( 3 );
    table_->horizontalHeader()->setLabel( 0, tools::translate( "gui::ResourceLinksDialog_ABC", "Target" ) );
    table_->horizontalHeader()->setLabel( 1, tools::translate( "gui::ResourceLinksDialog_ABC", "Limited" ) );
    table_->horizontalHeader()->setLabel( 2, tools::translate( "gui::ResourceLinksDialog_ABC", "Capacity" ) );
    table_->setColumnWidth( 1, 50 );
    connect( table_, SIGNAL( valueChanged( int, int ) ), SLOT( OnValueChanged( int, int ) ) );
    QPushButton* okBtn = new QPushButton( tools::translate( "gui::ResourceLinksDialog_ABC", "Validate" ), pMainLayout_ );
    okBtn->setDefault( true );
    connect( okBtn, SIGNAL( clicked() ), SLOT( Validate() ) );
    controllers_.Register( *this );
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
    selected_ = 0;
    selectedItem_ = 0;
    pMainLayout_->hide();
}

// -----------------------------------------------------------------------------
// Name: ResourceLinksDialog_ABC::AfterSelection
// Created: JSR 2010-09-09
// -----------------------------------------------------------------------------
void ResourceLinksDialog_ABC::AfterSelection()
{
    if( selected_ )
        Show();
}

// -----------------------------------------------------------------------------
// Name: ResourceLinksDialog_ABC::Select
// Created: JSR 2010-09-09
// -----------------------------------------------------------------------------
void ResourceLinksDialog_ABC::Select( const TerrainObjectProxy& proxy )
{
    selected_ = const_cast< TerrainObjectProxy& >( proxy ).Retrieve< ResourceNetwork_ABC >();
    id_ = proxy.GetId();
    urban_ = true;
}

// -----------------------------------------------------------------------------
// Name: ResourceLinksDialog_ABC::Select
// Created: JSR 2010-09-09
// -----------------------------------------------------------------------------
void ResourceLinksDialog_ABC::Select( const Object_ABC& object )
{
    selected_ = const_cast< Object_ABC& >( object ).Retrieve< ResourceNetwork_ABC >();
    id_ = object.GetId();
    urban_ = false;
}

// -----------------------------------------------------------------------------
// Name: ResourceLinksDialog_ABC::Update
// Created: JSR 2010-08-24
// -----------------------------------------------------------------------------
void ResourceLinksDialog_ABC::Update()
{
    QListBoxItem* item = dotationList_->selectedItem();
    if( !item )
    {
        dotationList_->setSelected( selectedItem_, true );
        return;
    }
    selectedItem_ = item;
    std::string resource = item->text().ascii();
    ResourceNetwork_ABC::ResourceNode& node = resourceNodes_[ resource ];
    groupBox_->setChecked( node.isEnabled_ );
    production_->setValue( node.production_ );
    consumption_->setValue( node.consumption_ );
    critical_->setChecked( node.critical_ );
    maxStock_->setValue( node.maxStock_ );
    stock_->setValue( node.stock_ );
    table_->setNumRows( node.links_.size() );
    table_->setColumnReadOnly( 0, true );
    for( unsigned int j = 0; j < node.links_.size(); ++j )
    {
        table_->setText( j, 0, selected_->GetLinkName( resource, j ) );
        table_->setItem( j, 1, new QCheckTableItem( table_, ""  ) );
        table_->setItem( j, 2, new SpinTableItem( table_, 0, std::numeric_limits< int >::max() ) );
        bool limited = node.links_[ j ].capacity_ != -1;
        table_->item( j, 2 )->setEnabled( limited );
        static_cast< QCheckTableItem* >( table_->item( j, 1 ) )->setChecked( limited );
        table_->setText( j, 2, QString::number( limited ? node.links_[ j ].capacity_ : 0 ) );
    }
}

// -----------------------------------------------------------------------------
// Name: ResourceLinksDialog_ABC::OnActivationChanged
// Created: JSR 2010-09-02
// -----------------------------------------------------------------------------
void ResourceLinksDialog_ABC::OnActivationChanged( bool on )
{
    resourceNodes_[ dotationList_->selectedItem()->text().ascii() ].isEnabled_ = on;
}

// -----------------------------------------------------------------------------
// Name: ResourceLinksDialog_ABC::OnProductionChanged
// Created: JSR 2010-09-02
// -----------------------------------------------------------------------------
void ResourceLinksDialog_ABC::OnProductionChanged( int value )
{
    resourceNodes_[ dotationList_->selectedItem()->text().ascii() ].production_ = value;
}

// -----------------------------------------------------------------------------
// Name: ResourceLinksDialog_ABC::OnConsumptionChanged
// Created: JSR 2010-09-02
// -----------------------------------------------------------------------------
void ResourceLinksDialog_ABC::OnConsumptionChanged( int value )
{
    resourceNodes_[ dotationList_->selectedItem()->text().ascii() ].consumption_ = value;
}

// -----------------------------------------------------------------------------
// Name: ResourceLinksDialog_ABC::OnCriticalChanged
// Created: JSR 2010-09-02
// -----------------------------------------------------------------------------
void ResourceLinksDialog_ABC::OnCriticalChanged( bool on )
{
    resourceNodes_[ dotationList_->selectedItem()->text().ascii() ].critical_ = on;
}

// -----------------------------------------------------------------------------
// Name: ResourceLinksDialog_ABC::OnMaxStockChanged
// Created: JSR 2010-09-08
// -----------------------------------------------------------------------------
void ResourceLinksDialog_ABC::OnMaxStockChanged( int value )
{
    resourceNodes_[ dotationList_->selectedItem()->text().ascii() ].maxStock_ = value;
}

// -----------------------------------------------------------------------------
// Name: ResourceLinksDialog_ABC::OnStockChanged
// Created: JSR 2010-09-02
// -----------------------------------------------------------------------------
void ResourceLinksDialog_ABC::OnStockChanged( int value )
{
    resourceNodes_[ dotationList_->selectedItem()->text().ascii() ].stock_ = value;
}

// -----------------------------------------------------------------------------
// Name: ResourceLinksDialog_ABC::OnValueChanged
// Created: JSR 2010-08-30
// -----------------------------------------------------------------------------
void ResourceLinksDialog_ABC::OnValueChanged( int, int )
{
    std::string resource = dotationList_->selectedItem()->text().ascii();
    for( int j = 0; j < table_->numRows(); ++j )
    {
        QCheckTableItem* item = static_cast< QCheckTableItem* >( table_->item( j, 1 ) );
        if( item )
        {
            bool enable = item->isChecked();
            table_->item( j, 2 )->setEnabled( enable );
            resourceNodes_[ resource ].links_[ j ].capacity_ = enable ? table_->text( j, 2 ).toInt() : -1;
        }
    }
}

// -----------------------------------------------------------------------------
// Name: ResourceLinksDialog_ABC::Validate
// Created: JSR 2010-08-24
// -----------------------------------------------------------------------------
void ResourceLinksDialog_ABC::Validate()
{
    // in case spin boxes have not been validated
    std::string resource = dotationList_->selectedItem()->text().ascii();
    resourceNodes_[ resource ].production_ = production_->value();
    resourceNodes_[ resource ].consumption_ = consumption_->value();
    resourceNodes_[ resource ].maxStock_ = maxStock_->value();
    resourceNodes_[ resource ].stock_ = stock_->value();
    DoValidate();
    controllers_.controller_.Update( *selected_ );
}

// -----------------------------------------------------------------------------
// Name: ResourceLinksDialog_ABC::NotifyDeleted
// Created: JSR 2010-09-09
// -----------------------------------------------------------------------------
void ResourceLinksDialog_ABC::NotifyDeleted( const Entity_ABC& element )
{
    if( selected_ == element.Retrieve< ResourceNetwork_ABC >() )
    {
        pMainLayout_->hide();
        selected_ = 0;
    }
}

// -----------------------------------------------------------------------------
// Name: ResourceLinksDialog_ABC::Show
// Created: JSR 2010-08-24
// -----------------------------------------------------------------------------
void ResourceLinksDialog_ABC::Show()
{
    dotationList_->clear();
    resourceNodes_ = selected_->GetResourceNodes();
    for( ResourceNetwork_ABC::CIT_ResourceNodes it = resourceNodes_.begin(); it != resourceNodes_.end(); ++it )
    {
        if( it == resourceNodes_.begin() )
        {
            selectedItem_ = new QListBoxText( dotationList_, it->second.resource_.c_str() );
            dotationList_->setSelected( selectedItem_, true );
        }
        else
            dotationList_->setSelected( new QListBoxText( dotationList_, it->second.resource_.c_str() ), false );
    }
    Update();
    pMainLayout_->show();
}
