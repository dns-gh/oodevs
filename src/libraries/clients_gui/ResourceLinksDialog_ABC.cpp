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
    setMinimumSize( 400, 300 );
    setCaption( tools::translate( "ResourceLinksDialog", "Resource Links" ) );
    QVBoxLayout* pMainLayout = new QVBoxLayout( this );
    pMainLayout->setSpacing( 5 );
    pMainLayout->setMargin( 5 );

    for( int i = 0; i < eNbrResourceType; ++i )
    {
        label_[ i ] = new QLabel( ConvertFromResourceType( static_cast< E_ResourceType >( i ) ), this );
        table_[ i ] = new QTable( this );
        table_[ i ]->setSelectionMode( QTable::NoSelection );
        table_[ i ]->setNumCols( 2 );
        table_[ i ]->horizontalHeader()->setLabel( 0, tools::translate( "ResourceLinksDialog_ABC", "Target" ) );
        table_[ i ]->horizontalHeader()->setLabel( 1, tools::translate( "ResourceLinksDialog_ABC", "Capacity" ) );
        //table_[ i ]->setMinimumSize( 300, 200 );
        //table_[ i ]->setMinimumWidth( 200 );
        pMainLayout->addWidget( label_[ i ] );
        pMainLayout->addWidget( table_[ i ] );
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
        if( !node )
        {
            label_[ i ]->hide();
            table_[ i ]->hide();
        }
        else
        {
            label_[ i ]->show();
            table_[ i ]->show();
            table_[ i ]->setNumRows( node->links_.size() );
            table_[ i ]->setColumnReadOnly( 0, true );
            for( unsigned int j = 0; j < node->links_.size(); ++j )
            {
                table_[ i ]->setText( j, 0, selected_->GetLinkName( type, j ) );
                table_[ i ]->setText( j, 1, QString::number( node->links_[ j ].capacity_ ) );
                /*QLineEdit* edit = ( QLineEdit* )( table_[ i ]->cellWidget( j, 1 ) );
                edit->setValidator( new QIntValidator( edit ) );*/
            }
        }        
    }
}
