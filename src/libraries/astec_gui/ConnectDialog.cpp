//****************************************************************************
//
// $Created:  NLD 2002-01-03 $
// $Archive: /MVW_v10/Build/SDK/Light2/src/ConnectDialog.cpp $
// $Author: Ape $
// $Modtime: 23/09/04 17:19 $
// $Revision: 5 $
// $Workfile: ConnectDialog.cpp $
//
//*****************************************************************************

#include "astec_gui_pch.h"

#include "ConnectDialog.h"
#include "moc_ConnectDialog.cpp"
#include "astec_gaming/Network.h"
#include "MT/MT_Logger/MT_Logger_lib.h"

//-----------------------------------------------------------------------------
// Name: ConnectDialog constructor
// Created:  NLD 2002-01-03 
//-----------------------------------------------------------------------------
ConnectDialog::ConnectDialog( QWidget* pParent, Network& network )
    : QDialog ( pParent, 0, true ) 
    , network_( network )
{
    setCaption( tr("Paramètres de connection") );

    QGridLayout* pMainLayout = new QGridLayout( this, 3, 2 );
    pMainLayout->setMargin( 10 );
    pMainLayout->setSpacing( 10 );

    uint nCurRow = 0;
    
    // Host name
    pMainLayout->addWidget( new QLabel( tr("Nom de l'hôte"), this ), nCurRow, 0 );
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
    QPushButton* pOKButton     = new QPushButton( tr( "OK" )    , this );
    QPushButton* pCancelButton = new QPushButton( tr( "Annuler" ), this );
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
    QSettings settings;
    settings.setPath( "MASA", "Astec" );
    settings.beginGroup( "/ConnectWindow" );
    
    QStringList list;
    for( int n = 0; n < pHostNameComboBox_->count(); ++n )
        list.push_back( pHostNameComboBox_->text( n ) );
    settings.writeEntry( "/hosts", list, ';' );
    settings.writeEntry( "/index", pHostNameComboBox_->currentItem() );
    settings.endGroup();
}


//=============================================================================
// SLOTS
//=============================================================================

//-----------------------------------------------------------------------------
// Name: ConnectDialog::Validate
// Created:  NLD 2002-01-03 
//-----------------------------------------------------------------------------
void ConnectDialog::Validate()
{
    try
    {
        network_.Connect( std::string( pHostNameComboBox_->currentText() ), pPortSpinBox_->value() );
    }
    catch ( std::exception& e )
    {
        std::stringstream strMsgTitle;
        std::stringstream strMsg;
        strMsgTitle << "Non connecté à " << pHostNameComboBox_->currentText().ascii() << ":" << pPortSpinBox_->value();
        strMsg << "Raison :" << std::endl << e.what();
        MT_LOG_INFO( strMsgTitle.str().c_str(), 0, strMsg.str().c_str() );
        reject();
        return;
    }
    accept();
}

//-----------------------------------------------------------------------------
// Name: ConnectDialog::Validate
// Created:  NLD 2002-01-03 
//-----------------------------------------------------------------------------
void ConnectDialog::Reject()
{
    reject();
}


//-----------------------------------------------------------------------------
// Name: ConnectDialog::LoadDefaultConfig
// Created:  NLD 2002-01-03 
//-----------------------------------------------------------------------------
void ConnectDialog::LoadDefaultConfig()
{
    pPortSpinBox_->setValue( 10000 );

    QSettings settings;
    settings.setPath( "MASA", "Astec" );
    settings.beginGroup( "/ConnectWindow" );
    QStringList list = settings.readListEntry( "/hosts", ';' );
    int nIndex = settings.readNumEntry( "/index", 0 );
    settings.endGroup();

    if( list.empty() )
        list.push_back( "localhost" );

    pHostNameComboBox_->insertStringList( list );
    pHostNameComboBox_->setCurrentItem( nIndex );
}
