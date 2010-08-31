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
#include "clients_kernel/Profile_ABC.h"
#include "clients_kernel/ResourceNetwork_ABC.h"

using namespace gui;
using namespace kernel;
using namespace resource;

namespace
{
    QString ConvertFromResourceType( E_ResourceType type )
    {
        switch( type )
        {
        case eResourceType_Water:
            return tools::translate( "ResourceLinksDialog_ABC", "Water" );
        case eResourceType_Gaz:
            return tools::translate( "ResourceLinksDialog_ABC", "Gaz" );
        case eResourceType_Electricity:
            return tools::translate( "ResourceLinksDialog_ABC", "Electricity" );
        default:
            throw std::exception( "Unknown resource type" ); 
        }
    }
}

// -----------------------------------------------------------------------------
// Name: ResourceLinksDialog_ABC constructor
// Created: JSR 2010-08-24
// -----------------------------------------------------------------------------
ResourceLinksDialog_ABC::ResourceLinksDialog_ABC( QWidget* parent, Controllers& controllers, const kernel::Profile_ABC& profile )
    : QDialog( parent, "ResourceLinksDialog", true )
    , controllers_( controllers )
    , profile_( profile )
    , selected_( 0 )
{
    setSizePolicy( QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding );
    setMinimumSize( 600, 300 );
    setCaption( tools::translate( "ResourceLinksDialog", "Resource Links" ) );
    QVBoxLayout* pMainLayout = new QVBoxLayout( this );
    pMainLayout->setSpacing( 5 );
    pMainLayout->setMargin( 5 );
    for( int i = 0; i < eNbrResourceType; ++i )
    {
        widgets_[ i ].groupBox_ = new QGroupBox( 1, Qt::Horizontal, ConvertFromResourceType( static_cast< E_ResourceType >( i ) ), this );
        pMainLayout->addWidget( widgets_[ i ].groupBox_ );
        widgets_[ i ].groupBox_->setCheckable( true );
        {
            QHBox* box = new QHBox( widgets_[ i ].groupBox_ );
            new QLabel( tools::translate( "ResourceLinksDialog_ABC", "Production:" ), box );
            widgets_[ i ].production_ = new QSpinBox( 0, std::numeric_limits< int >::max(), 1, box );
        }
        {
            QHBox* box = new QHBox( widgets_[ i ].groupBox_ );
            new QLabel( tools::translate( "ResourceLinksDialog_ABC", "Consumption:" ), box );
            widgets_[ i ].consumption_  = new QSpinBox( 0, std::numeric_limits< int >::max(), 1, box );
        }
        widgets_[ i ].critical_ = new QCheckBox( tools::translate( "ResourceLinksDialog_ABC", "Vital consumption" ), widgets_[ i ].groupBox_ );
        {
            QHBox* box = new QHBox( widgets_[ i ].groupBox_ );
            new QLabel( tools::translate( "ResourceLinksDialog_ABC", "Maximal stock:" ), box );
            widgets_[ i ].stock_ = new QSpinBox( 0, std::numeric_limits< int >::max(), 1, box );
        }
        widgets_[ i ].table_ = new QTable( widgets_[ i ].groupBox_ );
        widgets_[ i ].table_->setSelectionMode( QTable::NoSelection );
        widgets_[ i ].table_->setNumCols( 3 );
        widgets_[ i ].table_->horizontalHeader()->setLabel( 0, tools::translate( "ResourceLinksDialog_ABC", "Target" ) );
        widgets_[ i ].table_->horizontalHeader()->setLabel( 1, tools::translate( "ResourceLinksDialog_ABC", "Limited" ) );
        widgets_[ i ].table_->horizontalHeader()->setLabel( 2, tools::translate( "ResourceLinksDialog_ABC", "Capacity" ) );
        connect( widgets_[ i ].table_, SIGNAL( valueChanged( int, int ) ), SLOT( OnValueChanged( int, int ) ) );
    }
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
// Name: ResourceLinksDialog_ABC::OnValueChanged
// Created: JSR 2010-08-30
// -----------------------------------------------------------------------------
void ResourceLinksDialog_ABC::OnValueChanged( int, int )
{
    for( int i = 0; i < eNbrResourceType; ++i )
    {
        if( !widgets_[ i ].table_->isShown() )
            continue;
        for( int j = 0; j < widgets_[ i ].table_->numRows(); ++j )
        {
            bool enable = static_cast< QCheckTableItem* >( widgets_[ i ].table_->item( j, 1 ) )->isChecked();
            widgets_[ i ].table_->item( j, 2 )->setEnabled( enable );
        }
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
}

// -----------------------------------------------------------------------------
// Name: ResourceLinksDialog_ABC::Reject
// Created: JSR 2010-08-24
// -----------------------------------------------------------------------------
void ResourceLinksDialog_ABC::Reject()
{
    reject();
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
// Name: ResourceLinksDialog_ABC::showEvent
// Created: JSR 2010-08-24
// -----------------------------------------------------------------------------
void ResourceLinksDialog_ABC::showEvent( QShowEvent* )
{
    UpdateTables();
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

// -----------------------------------------------------------------------------
// Name: ResourceLinksDialog_ABC::UpdateTable
// Created: JSR 2010-08-24
// -----------------------------------------------------------------------------
void ResourceLinksDialog_ABC::UpdateTables()
{
    for( int i = 0; i < eNbrResourceType; ++i )
    {
        E_ResourceType type = static_cast< E_ResourceType >( i );
        const ResourceNetwork_ABC::ResourceNode* node = selected_->FindResourceNode( type );
        if( node )
        {
            widgets_[ i ].groupBox_->show();
            widgets_[ i ].groupBox_->setChecked( node->isEnabled_ );
            widgets_[ i ].production_->setValue( node->production_ );
            widgets_[ i ].consumption_->setValue( node->consumption_ );
            widgets_[ i ].critical_->setChecked( node->critical_ );
            widgets_[ i ].stock_->setValue( node->maxStock_ );
            widgets_[ i ].table_->setNumRows( node->links_.size() );
            widgets_[ i ].table_->setColumnReadOnly( 0, true );
            for( unsigned int j = 0; j < node->links_.size(); ++j )
            {
                widgets_[ i ].table_->setText( j, 0, selected_->GetLinkName( type, j ) );
                widgets_[ i ].table_->setItem( j, 1, new QCheckTableItem( widgets_[ i ].table_, ""  ) );
                widgets_[ i ].table_->setItem( j, 2, new SpinTableItem( widgets_[ i ].table_, 0, std::numeric_limits< int >::max() ) );
                bool limited = node->links_[ j ].capacity_ != -1;
                widgets_[ i ].table_->item( j, 2 )->setEnabled( limited );
                static_cast< QCheckTableItem* >( widgets_[ i ].table_->item( j, 1 ) )->setChecked( limited );
                widgets_[ i ].table_->setText( j, 2, QString::number( limited ? node->links_[ j ].capacity_ : 0 ) );
            }
        }
        else
            widgets_[ i ].groupBox_->hide();
    }
}
