//****************************************************************************
//
// $Created:  NLD 2002-01-03 $
// $Archive: /MVW_v10/Build/SDK/MOS_Light/src/MOS_ConnectDialog.cpp $
// $Author: Nld $
// $Modtime: 10/09/04 11:40 $
// $Revision: 11 $
// $Workfile: MOS_ConnectDialog.cpp $
//
//*****************************************************************************

#include "MOS_Light_Pch.h"

#include "MOS_ConnectDialog.h"
#include "moc_MOS_ConnectDialog.cpp"

#ifndef MOS_USE_INLINE
#   include "MOS_ConnectDialog.inl"
#endif

#include "MOS_App.h"
#include "MOS_MOSServer.h"
#include "MOS_Config.h"
#include "MOS_App.h"
#include "MOS_MainWindow.h"
#include "MOS_SIMControl.h"
#include "MOS_Value.h"
#include "MOS_TestManager.h"

#include <qlayout.h>
#include <qlabel.h>
#include <qpushbutton.h>
#include <qcombobox.h>
#include <qspinbox.h>
#include <QCheckBox.h>
#include <QLineEdit.h>
#include <QFileDialog.h>


//-----------------------------------------------------------------------------
// Name: MOS_ConnectDialog constructor
// Created:  NLD 2002-01-03 
//-----------------------------------------------------------------------------
MOS_ConnectDialog::MOS_ConnectDialog( QWidget* pParent )
: QDialog           ( pParent ) 
, bTest_            ( false )
, bReplay_          ( false )
, bRec_             ( false )
, pCheckBoxTest_    ( 0 )
, pCheckBoxRep_     ( 0 )
, pNameOut_         ( 0 )
, pLabelOut_        ( 0 )
{
    setCaption( tr("Connection parameters") );


    QGridLayout* pMainLayout = new QGridLayout( this, 10, 5 );
    pMainLayout->setMargin( 10 );
    pMainLayout->setSpacing( 10 );

    uint nCurRow = 0;
    
    // Host name
    pMainLayout->addWidget( new QLabel( tr("Host name"), this ), nCurRow, 0 );
    pHostNameComboBox_ = new QComboBox( TRUE, this );
    pHostNameComboBox_->insertItem( "localhost" );
    pHostNameComboBox_->insertItem( "nld" );
    pHostNameComboBox_->setAutoCompletion( TRUE );
    pMainLayout->addWidget( pHostNameComboBox_, nCurRow, 1 );

    // Port
    ++ nCurRow;
    pPortSpinBox_ = new QSpinBox( 1, 65535, 1, this );
    pMainLayout->addWidget( new QLabel( tr("Port"), this ), nCurRow, 0 );
    pMainLayout->addWidget( pPortSpinBox_, nCurRow, 1 );


    // Test
    ++ nCurRow;
    pCheckBoxTest_ = new QCheckBox( "Debug Test", this );
    pCheckBoxTest_->setChecked( false );
    pMainLayout->addWidget( pCheckBoxTest_, nCurRow, 0 );
    connect( pCheckBoxTest_, SIGNAL( toggled( bool ) ), this, SLOT( SlotSetModeTest( bool ) ) );

    pCheckBoxValue_ = new MOS_Value( eTypeValueInt, this, std::string( "Min Random Time" ), MOSToolValueModif + MOSToolValueTime +  MOSToolValueSlide + MOSToolValueTitle );
    pCheckBoxValue_->SetValueMaxWidget( 600 * 10 );
    pCheckBoxValue_->SetValueMinWidget( 0 );
    pMainLayout->addWidget( pCheckBoxValue_, nCurRow, 1 );


    //Replay
    ++ nCurRow;
    pCheckBoxRep_ = new QCheckBox( "Debug Replay", this );
    pCheckBoxRep_->setChecked( false );
    pMainLayout->addWidget( pCheckBoxRep_, nCurRow, 0 );
    connect( pCheckBoxRep_, SIGNAL( toggled( bool ) ), this, SLOT( SlotSetModeReplay( bool ) ) );

    //Record
    ++ nCurRow;
    pCheckBoxRec_ = new QCheckBox( "Record", this );
    pCheckBoxRec_->setChecked( false );
    pMainLayout->addWidget( pCheckBoxRec_, nCurRow, 0 );
    connect( pCheckBoxRec_, SIGNAL( toggled( bool ) ), this, SLOT( SlotSetModeRec( bool ) ) );

    ++ nCurRow;
    QHBoxLayout* pInLayout = new QHBoxLayout( pMainLayout );
    pInLayout->setMargin( 10 );
    pLabelOut_ = new QLabel( "File Name :", this );
    pLabelOut_->setMaximumWidth( 200 );
    pButtonOutDir_ = new QPushButton( "...", this );
    pButtonOutDir_->setMaximumWidth( 20 );
    pNameOut_ = new QLineEdit( this );
    pNameOut_->setMinimumWidth( 200 );
    pInLayout->addWidget( pLabelOut_ );
    pInLayout->addWidget( pNameOut_ );
    pInLayout->addWidget( pButtonOutDir_ );
    connect( pButtonOutDir_, SIGNAL( released() ), SLOT( SlotSelectOutDir() ) );
    connect( pNameOut_    , SIGNAL( textChanged ( const  QString&) ), SLOT( SlotOutDirChanged( const  QString&) ) );
 
            

    // Buttons
    ++ nCurRow;
    QPushButton* pCancelButton = new QPushButton( tr("Cancel"), this );
    QPushButton* pOKButton     = new QPushButton( tr("OK")    , this );
    pMainLayout->addWidget( pCancelButton, nCurRow, 0 );
    pMainLayout->addWidget( pOKButton, nCurRow, 1     );
    pOKButton->setDefault( TRUE );



    connect( pCancelButton, SIGNAL( clicked() ), SLOT( Reject() ) );
    connect( pOKButton    , SIGNAL( clicked() ), SLOT( Validate() ) );

/*    pNameOut_->hide();
    pCheckBoxValue_->hide();
    pLabelOut_->hide();
    pButtonOutDir_->hide();
*/
    // Load default config
    LoadDefaultConfig();
}

//-----------------------------------------------------------------------------
// Name: MOS_ConnectDialog destructor
// Created:  NLD 2002-01-03 
//-----------------------------------------------------------------------------
MOS_ConnectDialog::~MOS_ConnectDialog()
{
    
}

//=============================================================================
// SLOTS
//=============================================================================


//-----------------------------------------------------------------------------
// Name: MOS_ConnectDialog::SlotSelectOutDir
// Created: FBD 03-08-08
//-----------------------------------------------------------------------------
void MOS_ConnectDialog::SlotSelectOutDir()
{
    QString strName = QFileDialog::getSaveFileName( MOS_App::GetApp().GetTestManager().GetFileNameOut().c_str() );
    if ( strName != "" )
    {
        std::string sName( strName );
        for ( uint i = 0 ; i < sName.size() ; ++i )
        {
            if ( sName[i] == '/' )
                sName[i] = '\\';
        }
        MOS_App::GetApp().GetTestManager().SetFileNameOut( sName );
        pNameOut_->setText( MOS_App::GetApp().GetTestManager().GetFileNameOut().c_str() );
    }
    
}

// -----------------------------------------------------------------------------
// Name: MOS_ConnectDialog::SlotOutDirChanged
// Created: NLD 2003-08-18
// -----------------------------------------------------------------------------
void MOS_ConnectDialog::SlotOutDirChanged( const QString& strName )
{
    std::string sName( strName );
    for ( uint i = 0 ; i < sName.size() ; ++i )
    {
        if ( sName[i] == '/' )
            sName[i] = '\\';
    }
    MOS_App::GetApp().GetTestManager().SetFileNameOut( sName );
}

//-----------------------------------------------------------------------------
// Name: MOS_ConnectDialog::SlotSetModeTest
// Created: FBD 03-07-31
//-----------------------------------------------------------------------------
void MOS_ConnectDialog::SlotSetModeTest( bool bTest )
{
    bTest_ = bTest;
    if ( bTest_)
    {
        pNameOut_->show();
        pCheckBoxValue_->show();
        pLabelOut_->show();
        pButtonOutDir_->show();

        if ( bReplay_ )
        {
            bReplay_= false;
            pCheckBoxRep_->setChecked( false );
        }

        if ( bRec_ )
        {
            bRec_= false;
            pCheckBoxRec_->setChecked( false );
        }
    }
    else
    {
/*        pNameOut_->hide();
        pCheckBoxValue_->hide();
        pLabelOut_->hide();
        pButtonOutDir_->hide();
*/
    }
}

//-----------------------------------------------------------------------------
// Name: MOS_ConnectDialog::SlotSetModeRec
// Created: FBD 03-07-31
//-----------------------------------------------------------------------------
void MOS_ConnectDialog::SlotSetModeRec( bool bRec )
{
    bRec_ = bRec;
    if ( bRec_)
    {
        if ( bReplay_ )
        {
            bReplay_= false;
            pCheckBoxRep_->setChecked( false );
        }
        if ( bTest_ )
        {
/*          pNameOut_->hide();
            pCheckBoxValue_->hide();
            pLabelOut_->hide();
            pButtonOutDir_->hide();
*/
            bTest_= false;
            pCheckBoxTest_->setChecked( false );
        }
    }
}


//-----------------------------------------------------------------------------
// Name: MOS_ConnectDialog::SlotSetModeReplay
// Created: FBD 03-07-31
//-----------------------------------------------------------------------------
void MOS_ConnectDialog::SlotSetModeReplay( bool bReplay )
{
    bReplay_ = bReplay;
    if ( bReplay_ )
    {
        if ( bTest_ )
        {
/*          pNameOut_->hide();
            pCheckBoxValue_->hide();
            pLabelOut_->hide();
            pButtonOutDir_->hide();
*/
            bTest_= false;
            pCheckBoxTest_->setChecked( false );
        }
        if ( bRec_ )
        {
            bRec_= false;
            pCheckBoxRec_->setChecked( false );
        }

    }
}

//-----------------------------------------------------------------------------
// Name: MOS_ConnectDialog::Validate
// Created:  NLD 2002-01-03 
//-----------------------------------------------------------------------------
void MOS_ConnectDialog::Validate()
{
    accept();

    MOS_App::GetApp().GetMOSServer().Connect( std::string( pHostNameComboBox_->currentText() ), pPortSpinBox_->value() );
    MOS_App::GetApp().GetTestManager().SetMinimumTimeTest( pCheckBoxValue_->GetValueWidget() );
    MOS_App::GetApp().GetTestManager().SetFlagTest( bTest_ );
    MOS_App::GetApp().GetTestManager().SetFlagReplay( bReplay_ );
    MOS_App::GetApp().GetTestManager().SetFlagRec( bRec_ );

}

//-----------------------------------------------------------------------------
// Name: MOS_ConnectDialog::Validate
// Created:  NLD 2002-01-03 
//-----------------------------------------------------------------------------
void MOS_ConnectDialog::Reject()
{
    MOS_App::GetApp().GetMainWindow().GetSimControl().NotifyConnection( false );
    reject();
}


//-----------------------------------------------------------------------------
// Name: MOS_ConnectDialog::LoadDefaultConfig
// Created:  NLD 2002-01-03 
//-----------------------------------------------------------------------------
void MOS_ConnectDialog::LoadDefaultConfig()
{
    pPortSpinBox_->setValue( MOS_DEFAULT_PORT_AS_MOS );
}


//-----------------------------------------------------------------------------
// Name: MOS_ConnectDialog::Initialize
// Created: FBD 03-09-16
//-----------------------------------------------------------------------------
void MOS_ConnectDialog::Initialize()
{
    assert( pNameOut_ );
    pNameOut_->setText(  MOS_App::GetApp().GetTestManager().GetFileNameOut().c_str() );
}



//-----------------------------------------------------------------------------
// Name: MOS_ConnectDialog::Terminate
// Created: FBD 03-09-16
//-----------------------------------------------------------------------------
void MOS_ConnectDialog::Terminate()
{
    assert( pNameOut_ );
    pNameOut_->setText(  MOS_App::GetApp().GetTestManager().GetFileNameOut().c_str() );
}

