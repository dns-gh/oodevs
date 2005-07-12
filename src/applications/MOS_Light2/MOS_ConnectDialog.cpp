//****************************************************************************
//
// $Created:  NLD 2002-01-03 $
// $Archive: /MVW_v10/Build/SDK/MOS_Light2/src/MOS_ConnectDialog.cpp $
// $Author: Ape $
// $Modtime: 23/09/04 17:19 $
// $Revision: 5 $
// $Workfile: MOS_ConnectDialog.cpp $
//
//*****************************************************************************

#include "MOS_Light2_Pch.h"

#include "MOS_ConnectDialog.h"
#include "moc_MOS_ConnectDialog.cpp"

#ifndef MOS_USE_INLINE
#   include "MOS_ConnectDialog.inl"
#endif

#include "MOS_App.h"
#include "MOS_MOSServer.h"


//-----------------------------------------------------------------------------
// Name: MOS_ConnectDialog constructor
// Created:  NLD 2002-01-03 
//-----------------------------------------------------------------------------
MOS_ConnectDialog::MOS_ConnectDialog( QWidget* pParent )
    : QDialog           ( pParent, 0, true ) 
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
// Name: MOS_ConnectDialog destructor
// Created:  NLD 2002-01-03 
//-----------------------------------------------------------------------------
MOS_ConnectDialog::~MOS_ConnectDialog()
{
    QSettings settings;
    settings.setPath( "MASA", "MOS_Light2" );
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
// Name: MOS_ConnectDialog::Validate
// Created:  NLD 2002-01-03 
//-----------------------------------------------------------------------------
void MOS_ConnectDialog::Validate()
{
    try
    {
        MOS_App::GetApp().GetMOSServer().Connect( std::string( pHostNameComboBox_->currentText() ), pPortSpinBox_->value() );
    }
    catch ( MT_Exception& e )
    {
        std::stringstream strMsgTitle;
        std::stringstream strMsg;
        strMsgTitle << "Non connecté à " << pHostNameComboBox_->currentText().ascii() << ":" << pPortSpinBox_->value();
        strMsg << "Raison :" << std::endl << e.GetWholeMessage();
        MT_LOG_INFO( strMsgTitle.str().c_str(), eDefault, strMsg.str().c_str() );
//        MT_LOG_INFO_MSG(  );   

        reject();
        return;
    }
    accept();
}

//-----------------------------------------------------------------------------
// Name: MOS_ConnectDialog::Validate
// Created:  NLD 2002-01-03 
//-----------------------------------------------------------------------------
void MOS_ConnectDialog::Reject()
{
    reject();
}


//-----------------------------------------------------------------------------
// Name: MOS_ConnectDialog::LoadDefaultConfig
// Created:  NLD 2002-01-03 
//-----------------------------------------------------------------------------
void MOS_ConnectDialog::LoadDefaultConfig()
{
    pPortSpinBox_->setValue( MOS_DEFAULT_PORT_AS_MOS );

    QSettings settings;
    settings.setPath( "MASA", "MOS_Light2" );
    settings.beginGroup( "/ConnectWindow" );
    QStringList list = settings.readListEntry( "/hosts", ';' );
    int nIndex = settings.readNumEntry( "/index", 0 );
    settings.endGroup();

    if( list.empty() )
        list.push_back( "localhost" );

    pHostNameComboBox_->insertStringList( list );
    pHostNameComboBox_->setCurrentItem( nIndex );
}
