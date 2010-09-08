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
#include "SpinTableItem.h"
#include "TerrainObjectProxy.h"
#include "tools.h"
#include "clients_kernel/Controllers.h"
#include "clients_kernel/DotationType.h"
#include "clients_kernel/Object_ABC.h"
#include "clients_kernel/Profile_ABC.h"
#include "clients_kernel/ResourceNetwork_ABC.h"

using namespace gui;
using namespace kernel;

namespace
{
    class ResourceItem : public QListViewItem
    {
    public:
        ResourceItem( QListView * parent, QString name, unsigned long id )
            : QListViewItem( parent, name)
            , id_( id )
        {}

    public:
        unsigned long GetId() const
        {
            return id_;
        }

    private:
        unsigned long id_;
    };
}

// -----------------------------------------------------------------------------
// Name: ResourceLinksDialog_ABC constructor
// Created: JSR 2010-08-24
// -----------------------------------------------------------------------------
ResourceLinksDialog_ABC::ResourceLinksDialog_ABC( QWidget* parent, Controllers& controllers, const tools::Resolver_ABC< kernel::DotationType >& dotationResolver, const kernel::Profile_ABC& profile )
    : QDialog          ( parent, "ResourceLinksDialog", true )
    , controllers_     ( controllers )
    , dotationResolver_( dotationResolver )
    , profile_         ( profile )
    , selected_        ( 0 )
{
    setSizePolicy( QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding );
    setMinimumSize( 600, 600 );
    setCaption( tools::translate( "ResourceLinksDialog", "Resource Links" ) );
    QVBoxLayout* pMainLayout = new QVBoxLayout( this );
    pMainLayout->setSpacing( 5 );
    pMainLayout->setMargin( 5 );
    QHBoxLayout* pNodeLayout = new QHBoxLayout( pMainLayout );
    dotationList_ = new QListView( this );
    dotationList_->addColumn( tools::translate( "ResourceLinksDialog", "Resource" ) );
    connect( dotationList_, SIGNAL( selectionChanged() ), this, SLOT( Update() ) );
    pNodeLayout->addWidget( dotationList_ );
    groupBox_ = new QGroupBox( 1, Qt::Horizontal, "", this );
    pNodeLayout->addWidget( groupBox_ );
    groupBox_->setCheckable( true );
    connect( groupBox_, SIGNAL( toggled( bool ) ), this, SLOT( OnActivationChanged( bool ) ) );
    {
        QHBox* box = new QHBox( groupBox_ );
        new QLabel( tools::translate( "ResourceLinksDialog_ABC", "Production:" ), box );
        production_ = new QSpinBox( 0, std::numeric_limits< int >::max(), 1, box );
        connect( production_, SIGNAL( valueChanged( int ) ), this, SLOT( OnProductionChanged( int ) ) );
    }
    {
        QHBox* box = new QHBox( groupBox_ );
        new QLabel( tools::translate( "ResourceLinksDialog_ABC", "Consumption:" ), box );
        consumption_  = new QSpinBox( 0, std::numeric_limits< int >::max(), 1, box );
        connect( consumption_, SIGNAL( valueChanged( int ) ), this, SLOT( OnConsumptionChanged( int ) ) );
    }
    critical_ = new QCheckBox( tools::translate( "ResourceLinksDialog_ABC", "Vital consumption" ), groupBox_ );
    connect( critical_, SIGNAL( toggled( bool ) ), this, SLOT( OnCriticalChanged( bool ) ) );
    {
        QHBox* box = new QHBox( groupBox_ );
        new QLabel( tools::translate( "ResourceLinksDialog_ABC", "Maximal stock:" ), box );
        maxStock_ = new QSpinBox( 0, std::numeric_limits< int >::max(), 1, box );
        connect( maxStock_, SIGNAL( valueChanged( int ) ), this, SLOT( OnMaxStockChanged( int ) ) );
    }
    {
        QHBox* box = new QHBox( groupBox_ );
        new QLabel( tools::translate( "ResourceLinksDialog_ABC", "Stock:" ), box );
        stock_ = new QSpinBox( 0, std::numeric_limits< int >::max(), 1, box );
        connect( stock_, SIGNAL( valueChanged( int ) ), this, SLOT( OnStockChanged( int ) ) );
    }
    table_ = new QTable( groupBox_ );
    table_->setSelectionMode( QTable::NoSelection );
    table_->setNumCols( 3 );
    table_->horizontalHeader()->setLabel( 0, tools::translate( "ResourceLinksDialog_ABC", "Target" ) );
    table_->horizontalHeader()->setLabel( 1, tools::translate( "ResourceLinksDialog_ABC", "Limited" ) );
    table_->horizontalHeader()->setLabel( 2, tools::translate( "ResourceLinksDialog_ABC", "Capacity" ) );
    connect( table_, SIGNAL( valueChanged( int, int ) ), SLOT( OnValueChanged( int, int ) ) );
    {
        QHBoxLayout* pButtonLayout = new QHBoxLayout( pMainLayout );
        pButtonLayout->setAlignment( Qt::Right );
        QPushButton* okBtn     = new QPushButton( tools::translate( "ResourceLinksDialog_ABC", "Ok" ), this );
        QPushButton* cancelBtn = new QPushButton( tools::translate( "ResourceLinksDialog_ABC", "Cancel" ), this );
        pButtonLayout->addWidget( okBtn );
        pButtonLayout->addWidget( cancelBtn );
        okBtn->setDefault( true );
        okBtn->setMaximumWidth( 100 );
        cancelBtn->setMaximumWidth( 100 );
        connect( cancelBtn, SIGNAL( clicked() ), SLOT( Reject() ) );
        connect( okBtn    , SIGNAL( clicked() ), SLOT( Validate() ) );
    }
    controllers_.Register( *this );
    hide();
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
// Name: ResourceLinksDialog_ABC::Update
// Created: JSR 2010-08-24
// -----------------------------------------------------------------------------
void ResourceLinksDialog_ABC::Update()
{
    unsigned long id = static_cast< ResourceItem* >( dotationList_->selectedItem() )->GetId();
    ResourceNetwork_ABC::ResourceNode& node = resourceNodes_[ id ];
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
        table_->setText( j, 0, selected_->GetLinkName( id, j ) );
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
    unsigned long id = static_cast< ResourceItem* >( dotationList_->selectedItem() )->GetId();
    resourceNodes_[ id ].isEnabled_ = on;
}
    
// -----------------------------------------------------------------------------
// Name: ResourceLinksDialog_ABC::OnProductionChanged
// Created: JSR 2010-09-02
// -----------------------------------------------------------------------------
void ResourceLinksDialog_ABC::OnProductionChanged( int value )
{
    unsigned long id = static_cast< ResourceItem* >( dotationList_->selectedItem() )->GetId();
    resourceNodes_[ id ].production_ = value;
}

// -----------------------------------------------------------------------------
// Name: ResourceLinksDialog_ABC::OnConsumptionChanged
// Created: JSR 2010-09-02
// -----------------------------------------------------------------------------
void ResourceLinksDialog_ABC::OnConsumptionChanged( int value )
{
    unsigned long id = static_cast< ResourceItem* >( dotationList_->selectedItem() )->GetId();
    resourceNodes_[ id ].consumption_ = value;
}
    
// -----------------------------------------------------------------------------
// Name: ResourceLinksDialog_ABC::OnCriticalChanged
// Created: JSR 2010-09-02
// -----------------------------------------------------------------------------
void ResourceLinksDialog_ABC::OnCriticalChanged( bool on )
{
    unsigned long id = static_cast< ResourceItem* >( dotationList_->selectedItem() )->GetId();
    resourceNodes_[ id ].critical_ = on;
}

// -----------------------------------------------------------------------------
// Name: ResourceLinksDialog_ABC::OnMaxStockChanged
// Created: JSR 2010-09-08
// -----------------------------------------------------------------------------
void ResourceLinksDialog_ABC::OnMaxStockChanged( int value )
{
    unsigned long id = static_cast< ResourceItem* >( dotationList_->selectedItem() )->GetId();
    resourceNodes_[ id ].maxStock_ = value;
}

// -----------------------------------------------------------------------------
// Name: ResourceLinksDialog_ABC::OnStockChanged
// Created: JSR 2010-09-02
// -----------------------------------------------------------------------------
void ResourceLinksDialog_ABC::OnStockChanged( int value )
{
    unsigned long id = static_cast< ResourceItem* >( dotationList_->selectedItem() )->GetId();
    resourceNodes_[ id ].stock_ = value;
}

// -----------------------------------------------------------------------------
// Name: ResourceLinksDialog_ABC::OnValueChanged
// Created: JSR 2010-08-30
// -----------------------------------------------------------------------------
void ResourceLinksDialog_ABC::OnValueChanged( int, int )
{
    unsigned long id = static_cast< ResourceItem* >( dotationList_->selectedItem() )->GetId();
    for( int j = 0; j < table_->numRows(); ++j )
    {
        bool enable = static_cast< QCheckTableItem* >( table_->item( j, 1 ) )->isChecked();
        table_->item( j, 2 )->setEnabled( enable );
        resourceNodes_[ id ].links_[ j ].capacity_ = enable ? table_->text( j, 2 ).toInt() : -1;
    }
}

// -----------------------------------------------------------------------------
// Name: ResourceLinksDialog_ABC::Validate
// Created: JSR 2010-08-24
// -----------------------------------------------------------------------------
void ResourceLinksDialog_ABC::Validate()
{
    accept();
    DoValidate();
    resourceNodes_.clear();
    dotationList_->clear();
    controllers_.controller_.Update( *selected_ );
}

// -----------------------------------------------------------------------------
// Name: ResourceLinksDialog_ABC::Reject
// Created: JSR 2010-08-24
// -----------------------------------------------------------------------------
void ResourceLinksDialog_ABC::Reject()
{
    reject();
    resourceNodes_.clear();
    dotationList_->clear();
}

// -----------------------------------------------------------------------------
// Name: ResourceLinksDialog_ABC::NotifyContextMenu
// Created: JSR 2010-08-24
// -----------------------------------------------------------------------------
void ResourceLinksDialog_ABC::NotifyContextMenu( const TerrainObjectProxy& proxy, ContextMenu& menu )
{
    if( profile_.CanDoMagic( proxy ) && ( selected_ = proxy.Retrieve< ResourceNetwork_ABC >() ) != 0 )
    {
        menu.InsertItem( "Command", tools::translate( "ResourceLinksDialog_ABC", "Resource links" ), this, SLOT( show() ) );
        urban_ = true;
        id_ = proxy.GetId();
    }
}

// -----------------------------------------------------------------------------
// Name: ResourceLinksDialog_ABC::NotifyContextMenu
// Created: JSR 2010-09-01
// -----------------------------------------------------------------------------
void ResourceLinksDialog_ABC::NotifyContextMenu( const kernel::Object_ABC& object, kernel::ContextMenu& menu )
{
    if( profile_.CanDoMagic( object ) && ( selected_ = object.Retrieve< ResourceNetwork_ABC >() ) != 0 )
    {
        menu.InsertItem( "Command", tools::translate( "ResourceLinksDialog_ABC", "Resource links" ), this, SLOT( show() ) );
        urban_ = false;
        id_ = object.GetId();
    }
}

// -----------------------------------------------------------------------------
// Name: ResourceLinksDialog_ABC::showEvent
// Created: JSR 2010-08-24
// -----------------------------------------------------------------------------
void ResourceLinksDialog_ABC::showEvent( QShowEvent* )
{
    dotationList_->clear();
    resourceNodes_ = selected_->GetResourceNodes();
    for( ResourceNetwork_ABC::CIT_ResourceNodes it = resourceNodes_.begin(); it != resourceNodes_.end(); ++it )
    {
        unsigned long id = it->second.resource_;
        dotationList_->setSelected( new ResourceItem( dotationList_, dotationResolver_.Get( id ).GetCategory().c_str(), id ), it == resourceNodes_.begin() );
    }
    Update();
}

// -----------------------------------------------------------------------------
// Name: ResourceLinksDialog_ABC::sizeHint
// Created: JSR 2010-08-24
// -----------------------------------------------------------------------------
QSize ResourceLinksDialog_ABC::sizeHint()
{
    // TODO ?
    return QDialog::sizeHint();
}
