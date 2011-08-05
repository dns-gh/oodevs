// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_app_pch.h"
#include "ConnectDialog.h"
#include "moc_ConnectDialog.cpp"
#include "gaming/Network.h"
#include "clients_kernel/Logger_ABC.h"
#include "clients_gui/Tools.h"

//-----------------------------------------------------------------------------
// Name: ConnectDialog constructor
// Created:  NLD 2002-01-03
//-----------------------------------------------------------------------------
ConnectDialog::ConnectDialog( QWidget* pParent, Network& network, kernel::Logger_ABC& logger )
: QDialog ( pParent )
    , network_( network )
    , logger_ ( logger )
{
    setCaption( tr("Connection parameters") );
    setModal( true );

    Q3GridLayout* pMainLayout = new Q3GridLayout( this, 3, 2 );
    pMainLayout->setMargin( 10 );
    pMainLayout->setSpacing( 10 );

    uint nCurRow = 0;

    // Host name
    pMainLayout->addWidget( new QLabel( tr("Host name"), this ), nCurRow, 0 );
    pHostNameComboBox_ = new QComboBox( TRUE, this );
    pHostNameComboBox_->setMaxCount( 5 );
    pHostNameComboBox_->setAutoCompletion( TRUE );
    pMainLayout->addWidget( pHostNameComboBox_, nCurRow, 1 );

    // Port
    ++ nCurRow;
    pPortSpinBox_ = new QSpinBox( 1, 65535, 1, this );
    pMainLayout->addWidget( new QLabel( tr("Port"), this ), nCurRow, 0 );
    pMainLayout->addWidget( pPortSpinBox_, nCurRow, 1 );

    // Buttons
    ++ nCurRow;
    QPushButton* pOKButton     = new QPushButton( tr( "Ok" )    , this );
    QPushButton* pCancelButton = new QPushButton( tr( "Cancel" ), this );
    pMainLayout->addWidget( pOKButton, nCurRow, 0     );
    pMainLayout->addWidget( pCancelButton, nCurRow, 1 );
    pOKButton->setDefault( TRUE );

    connect( pCancelButton, SIGNAL( clicked() ), SLOT( reject() ) );
    connect( pOKButton    , SIGNAL( clicked() ), SLOT( Validate() ) );

    // Load default config
    LoadDefaultConfig();
}

//-----------------------------------------------------------------------------
// Name: ConnectDialog destructor
// Created:  NLD 2002-01-03
//-----------------------------------------------------------------------------
ConnectDialog::~ConnectDialog()
{
    // NOTHING
}

//-----------------------------------------------------------------------------
// Name: ConnectDialog::Validate
// Created:  NLD 2002-01-03
//-----------------------------------------------------------------------------
void ConnectDialog::Validate()
{
    try
    {
        network_.Connect( std::string( pHostNameComboBox_->currentText() ), unsigned short( pPortSpinBox_->value() ) );
        SaveConfig();
        accept();
    }
    catch( std::exception& e )
    {
        logger_.Info() << tr( "Not connected to %1:%2\nReason: %3\n" ).arg( pHostNameComboBox_->currentText() )
                                                                      .arg( pPortSpinBox_->value() )
                                                                      .arg( e.what() );
        reject();
    }
}

//-----------------------------------------------------------------------------
// Name: ConnectDialog::LoadDefaultConfig
// Created:  NLD 2002-01-03
//-----------------------------------------------------------------------------
void ConnectDialog::LoadDefaultConfig()
{
    pPortSpinBox_->setValue( 10001 );
    QSettings settings;
    settings.setPath( "MASA Group", tools::translate( "Application", "SWORD" ) );
    settings.beginGroup( "/Common/Hosts" );
    QStringList list = settings.readListEntry( "/hosts", ';' );
    int nIndex = settings.readNumEntry( "/index", 0 );
    settings.endGroup();
    if( list.empty() )
        list.push_back( "localhost" );
    const QString currentItem = ( nIndex > 0 && nIndex < int( list.count() ) ) ? list[ nIndex ] : list[0];
    list.sort();
    QStringList uniqList;
    for( QStringList::const_iterator it = list.begin(); it != list.end(); ++it )
        if( uniqList.isEmpty() || *it != uniqList.back() )
            uniqList.append( *it );
    pHostNameComboBox_->insertStringList( uniqList );
    pHostNameComboBox_->setCurrentText( currentItem );
}

// -----------------------------------------------------------------------------
// Name: ConnectDialog::SaveConfig
// Created: SBO 2007-03-12
// -----------------------------------------------------------------------------
void ConnectDialog::SaveConfig()
{
    QSettings settings;
    settings.setPath( "MASA Group", tools::translate( "Application", "SWORD" ) );
    settings.beginGroup( "/Common/Hosts" );
    QStringList list;
    for( int n = 0; n < pHostNameComboBox_->count(); ++n )
        list.push_back( pHostNameComboBox_->text( n ) );
    settings.writeEntry( "/hosts", list, ';' );
    settings.writeEntry( "/index", pHostNameComboBox_->currentItem() );
    settings.endGroup();
}

// -----------------------------------------------------------------------------
// Name: ConnectDialog::FillPopupMenu
// Created: SBO 2008-09-18
// -----------------------------------------------------------------------------
void ConnectDialog::FillPopupMenu( Q3PopupMenu* menu )
{
    const QString port = QString::number( pPortSpinBox_->value() );
    for( int n = 0; n < pHostNameComboBox_->count(); ++n )
        menu->insertItem( pHostNameComboBox_->text( n ) + ":" + port, this, SLOT( QuickConnect( int ) ) );
}

// -----------------------------------------------------------------------------
// Name: ConnectDialog::QuickConnect
// Created: SBO 2007-03-26
// -----------------------------------------------------------------------------
void ConnectDialog::QuickConnect( int index )
{
    pHostNameComboBox_->setCurrentItem( index );
    Validate();
}
