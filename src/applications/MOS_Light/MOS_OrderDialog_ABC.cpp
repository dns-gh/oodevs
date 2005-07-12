//****************************************************************************
//
// $Created:  NLD 2002-01-03 $
// $Archive: /MVW_v10/Build/SDK/MOS_Light/src/MOS_OrderDialog_ABC.cpp $
// $Author: Nld $
// $Modtime: 10/09/04 11:40 $
// $Revision: 14 $
// $Workfile: MOS_OrderDialog_ABC.cpp $
//
//*****************************************************************************

#include "MOS_Light_Pch.h"

#include "MOS_OrderDialog_ABC.h"
#include "moc_MOS_OrderDialog_ABC.cpp"

#ifndef MOS_USE_INLINE
#   include "MOS_OrderDialog_ABC.inl"
#endif

#include "MOS_App.h"
#include "MOS_MOSServer.h"
#include "MOS_Config.h"
#include "MOS_Agent.h"
#include "MOS_LineManager.h"
#include "MOS_Limit.h"
#include "MOS_Lima.h"
#include <qlayout.h>
#include <qlabel.h>
#include <qpushbutton.h>
#include <qcombobox.h>
#include <qlistview.h>
#include <qpopupmenu.h>
#include <qaction.h>
#include <qcursor.h>
#include <qgroupbox.h>

using namespace DIN;


//$$$$ TMP
class QLimaFuncListViewItem : public QListViewItem
{
public:
    QLimaFuncListViewItem( QListViewItem* pParent, int nFunction, const char* szFuncName, const char* szValue )
        : QListViewItem( pParent, MT_FormatString( "%s - %s", szFuncName, szValue ).c_str() )
        , nFunction_( nFunction )
        , bUseValue_( true )
    {
        nValue_ = atoi( szValue );
    }

    QLimaFuncListViewItem( QListViewItem* pParent, int nFunction, const char* szFuncName )
        : QListViewItem( pParent, szFuncName )
        , nFunction_( nFunction )
        , bUseValue_( false )
    {
    }

public:
    int  nFunction_;
    int  nValue_;
    bool bUseValue_;
};

//-----------------------------------------------------------------------------
// Name: MOS_OrderDialog_ABC constructor
// Created:  NLD 2002-01-03 
//-----------------------------------------------------------------------------
MOS_OrderDialog_ABC::MOS_OrderDialog_ABC( QWidget* pParent )
    : MOS_AgentDialog_ABC( pParent )
{
    /////////////////////////////////////////
    // Main layout
    pMainLayout_ = new QVBoxLayout( this );
    pMainLayout_->setMargin( 2 );
    pMainLayout_->setSpacing( 10 );

    /////////////////////////////////////////
    // First layout
    pFirstGroupBox_ = new QGroupBox( "Common part", this );
    pMainLayout_->addWidget( pFirstGroupBox_ );

    QGridLayout* pFirstLayout = new QGridLayout( pFirstGroupBox_, 5, 5 );
    pFirstLayout->setMargin( 20 );

    int nCurRow = 0;

    //--------------------
    // Order type
    pFirstLayout->addWidget( new QLabel( tr("Order type" ), pFirstGroupBox_ ), nCurRow, 0 );
    pOrderTypeComboBox_ = new QComboBox( FALSE, pFirstGroupBox_ );
    pOrderTypeComboBox_->insertItem( "Ordre initial" );
    pOrderTypeComboBox_->insertItem( "Ordre de conduite" );
    pFirstLayout->addWidget( pOrderTypeComboBox_, nCurRow, 1 );
    pFirstLayout->addRowSpacing( (++nCurRow)++, 20 );

    //--------------------
    // Limits
    pFirstLayout->addWidget( new QLabel( tr("Left limit" ), pFirstGroupBox_ ), nCurRow, 0 );
    pLeftLimitComboBox_ = new QComboBox( FALSE, pFirstGroupBox_ );
    pFirstLayout->addWidget( pLeftLimitComboBox_, nCurRow, 1 );
    pFirstLayout->addRowSpacing( ++nCurRow, 20 );
    pFirstLayout->addWidget( new QLabel( tr("Right limit" ), pFirstGroupBox_ ), nCurRow, 0 );
    pRightLimitComboBox_ = new QComboBox( FALSE, pFirstGroupBox_ );
    pFirstLayout->addWidget( pRightLimitComboBox_, nCurRow, 1 );
    pFirstLayout->addRowSpacing( (++nCurRow)++, 20 );
    
    //--------------------
    // Limas   
    pFirstLayout->addWidget( new QLabel( tr("Limas" ), pFirstGroupBox_ ), nCurRow, 0 );
    pLimaListView_ = new QListView( pFirstGroupBox_ );
    pLimaListView_->addColumn( "Limas" );
    pLimaListView_->setRootIsDecorated( true );
    connect( pLimaListView_, SIGNAL( pressed( QListViewItem * ) ), this, SLOT( SlotLimaItemPressed( QListViewItem* ) ) );
    pFirstLayout->addMultiCellWidget( pLimaListView_, nCurRow, nCurRow + 1, 1, 1 );

    // Add lima
    QPushButton* pAddLimaButton = new QPushButton( tr("<- Add Lima"), pFirstGroupBox_ );
    pFirstLayout->addWidget( pAddLimaButton, nCurRow, 2 );
    connect( pAddLimaButton, SIGNAL( clicked() ), SLOT( SlotAddLima() ) );
    pLimaComboBox_ = new QComboBox( FALSE, pFirstGroupBox_ );
    pFirstLayout->addWidget( pLimaComboBox_, nCurRow, 3 );

    nCurRow++;
    // Add lima function
    pAddLimaFunctionButton_ = new QPushButton( tr("<- Add Lima Function"), pFirstGroupBox_ );
    pFirstLayout->addWidget( pAddLimaFunctionButton_, nCurRow, 2 );
    pAddLimaFunctionButton_->setEnabled( false );
    connect( pAddLimaFunctionButton_, SIGNAL( clicked() ), SLOT( SlotAddLimaFunction() ) );
    pLimaFunctionComboBox_ = new QComboBox( FALSE, pFirstGroupBox_ );
    pLimaFunctionComboBox_->setEnabled( false );
    connect( pLimaFunctionComboBox_, SIGNAL( activated( int ) ), SLOT( SlotChangeLimaFunction( int ) ) );
    pFirstLayout->addWidget( pLimaFunctionComboBox_, nCurRow, 3 );
    
    // Value comboBox ($$$ TMP $$$)
    pLimaFunctionValueComboBox_ = new QComboBox( TRUE, pFirstGroupBox_ );
    pLimaFunctionValueComboBox_->insertItem( "515" );
    pLimaFunctionValueComboBox_->setEnabled( false );
    pFirstLayout->addWidget( pLimaFunctionValueComboBox_, nCurRow, 4 );
       
    // Context menu
    pLimaListViewMenu_ = new QPopupMenu( pFirstGroupBox_ );
    QAction* pRemoveItemAction = new QAction( tr("&Remove"), tr("&Remove"), 0, pFirstGroupBox_ );
    pRemoveItemAction->addTo( pLimaListViewMenu_ );
    connect( pRemoveItemAction, SIGNAL( activated() ), SLOT( SlotRemoveLimaOrLimaFunction() ) );
    connect( pLimaListView_, SIGNAL( contextMenuRequested( QListViewItem *, const QPoint &, int ) ), this, SLOT( SlotLimaMenuRequested( QListViewItem* ) ) );

    pFirstLayout->addRowSpacing( (++nCurRow)++, 20 );


    //--------------------
    // Start time
    pFirstLayout->addWidget( new QLabel( tr("Start time" ), pFirstGroupBox_ ), nCurRow, 0 );
    pStartTimeComboBox_ = new QComboBox( FALSE, pFirstGroupBox_ );
    pStartTimeComboBox_->insertItem( "65465" );
    pFirstLayout->addWidget( pStartTimeComboBox_, nCurRow, 1 );
    
    /////////////////////////////////////////            
    // Second layout
    pSecondGroupBox_ = new QGroupBox( "Specific part", this );
    pMainLayout_->addWidget( pSecondGroupBox_ );

      
    /////////////////////////////////////////
    // Third layout
    QHBoxLayout* pThirdLayout = new QHBoxLayout( pMainLayout_ );

    //--------------------
    // OK / Cancel
    QPushButton* pOKButton = new QPushButton( tr("OK"), this );
    pThirdLayout->addWidget( pOKButton );
    QPushButton* pCancelButton = new QPushButton( tr("Cancel"), this );
    pThirdLayout->addWidget( pCancelButton );
    pOKButton->setDefault( TRUE );
    
    connect( pCancelButton, SIGNAL( clicked() ), SLOT( SlotCancel() ) );
    connect( pOKButton    , SIGNAL( clicked() ), SLOT( SlotValidate() ) );
}

//-----------------------------------------------------------------------------
// Name: MOS_OrderDialog_ABC destructor
// Created:  NLD 2002-01-03 
//-----------------------------------------------------------------------------
MOS_OrderDialog_ABC::~MOS_OrderDialog_ABC()
{
    
}


//=============================================================================
// SLOTS
//=============================================================================


//-----------------------------------------------------------------------------
// Name: MOS_OrderDialog_ABC::SlotAddLima
// Created: NLD 2002-08-23
//-----------------------------------------------------------------------------
void MOS_OrderDialog_ABC::SlotAddLima()
{
    new QListViewItem( pLimaListView_, pLimaComboBox_->currentText() );
}


//-----------------------------------------------------------------------------
// Name: MOS_OrderDialog_ABC::SlotAddLimaFunction
// Created: NLD 2002-08-23
//-----------------------------------------------------------------------------
void MOS_OrderDialog_ABC::SlotAddLimaFunction()
{
    QListViewItem* pSelectedItem = pLimaListView_->selectedItem();

    if( !pSelectedItem || pSelectedItem->parent() )
        return;
    
    //$$$ TMP
    // Value associated
    if( pLimaFunctionValueComboBox_->isEnabled() )
        new QLimaFuncListViewItem( pSelectedItem, pLimaFunctionComboBox_->currentItem(), pLimaFunctionComboBox_->currentText(), pLimaFunctionValueComboBox_->currentText() );
    else
        new QLimaFuncListViewItem( pSelectedItem, pLimaFunctionComboBox_->currentItem(), pLimaFunctionComboBox_->currentText() );
}

//-----------------------------------------------------------------------------
// Name: MOS_OrderDialog_ABC::SlotRemoveLimaOrLimaFunction
// Created: NLD 2002-08-23
//-----------------------------------------------------------------------------
void MOS_OrderDialog_ABC::SlotRemoveLimaOrLimaFunction()
{
    if( pLimaListView_->selectedItem() )
        delete pLimaListView_->selectedItem();
}

//-----------------------------------------------------------------------------
// Name: MOS_OrderDialog_ABC::SlotLimaMenuRequested
// Created: NLD 2002-08-23
//-----------------------------------------------------------------------------
void MOS_OrderDialog_ABC::SlotLimaMenuRequested( QListViewItem *pItem )
{
    if( !pItem )
        return;

    pLimaListViewMenu_->move( QCursor::pos() );
    pLimaListViewMenu_->show();    
}


//-----------------------------------------------------------------------------
// Name: MOS_OrderDialog_ABC::SlotChangeLimaFunction
// Created: NLD 2002-08-23
//-----------------------------------------------------------------------------
void MOS_OrderDialog_ABC::SlotChangeLimaFunction( int nIdx )
{
    // Time coord
    if( nIdx == 3 )
        pLimaFunctionValueComboBox_->setEnabled( true );
    else
        pLimaFunctionValueComboBox_->setEnabled( false );
}


//-----------------------------------------------------------------------------
// Name: MOS_OrderDialog_ABC::SlotLimaItemPressed
// Created: NLD 2002-08-23
//-----------------------------------------------------------------------------
void MOS_OrderDialog_ABC::SlotLimaItemPressed( QListViewItem* pItem )
{
    if( !pItem || pItem->parent() )
    {
        pAddLimaFunctionButton_->setEnabled( false );
        pLimaFunctionComboBox_ ->setEnabled( false );
    }
    else
    {
        pAddLimaFunctionButton_->setEnabled( true );
        pLimaFunctionComboBox_ ->setEnabled( true );
    }
}

//-----------------------------------------------------------------------------
// Name: MOS_OrderDialog_ABC::WriteOrder
// Created: NLD 2002-08-23
//-----------------------------------------------------------------------------
/*DIN_BufferedMessage MOS_OrderDialog_ABC::WriteOrder()
{
	assert( false );
    assert( pAgent_ );

    //DIN_BufferedMessage msg = pAgent_->PrebuildOrderMsg();
/*
    // Type initial//Conduite $$$$ a virer + tard
    msg << (uint8)pOrderTypeComboBox_->currentItem();

    // Limits
    MIL_LimitID nLeftLimitID = atoi( pLeftLimitComboBox_->currentText() );
    MIL_LimitID nRightLimitID = atoi( pRightLimitComboBox_->currentText() );
    msg << nLeftLimitID;
    msg << nRightLimitID;

    // Limas
    msg << (uint8)pLimaListView_->childCount();
    QListViewItem* pLimaItem = pLimaListView_->firstChild();
    while( pLimaItem )
    {
        msg << (MIL_LimaID)atoi( pLimaItem->text( 0 ) );

        //$$$$ TMP
        uint8 nNbrLimaFunc = 0;
        QLimaFuncListViewItem* pLimaFuncItem = (QLimaFuncListViewItem*)pLimaItem->firstChild();
        while( pLimaFuncItem )
        {
            ++ nNbrLimaFunc;
            pLimaFuncItem = (QLimaFuncListViewItem*)pLimaFuncItem->nextSibling();
        }
        //$$$$ TMP

        msg << nNbrLimaFunc;
        
        pLimaFuncItem = (QLimaFuncListViewItem*)pLimaItem->firstChild();
        while( pLimaFuncItem )
        {
            msg << (MIL_LimaFunctionID)pLimaFuncItem->nFunction_;
            if( pLimaFuncItem->bUseValue_ )
                msg << (MIL_TimeID)pLimaFuncItem->nValue_;

            pLimaFuncItem = (QLimaFuncListViewItem*)pLimaFuncItem->nextSibling();
        }

        pLimaItem = pLimaItem->nextSibling();
    }

    // Hour
    msg << (MIL_TimeID)atoi( pStartTimeComboBox_->currentText() );

    return msg;
	
}*/


//=============================================================================
// INITIALIZATION
//=============================================================================

//-----------------------------------------------------------------------------
// Name: MOS_OrderDialog_ABC::Initialize
// Created: NLD 2002-08-23
//-----------------------------------------------------------------------------
void MOS_OrderDialog_ABC::Initialize( MOS_Agent& agent )
{
    MOS_AgentDialog_ABC::Initialize( agent );
    
    InitLimitComboBox       ( *pLeftLimitComboBox_ );
    InitLimitComboBox       ( *pRightLimitComboBox_ );
    InitLimaComboBox        ( *pLimaComboBox_ );
    InitLimaFunctionComboBox( *pLimaFunctionComboBox_ );
}


//-----------------------------------------------------------------------------
// Name: MOS_OrderDialog_ABC::Cleanup
// Created: NLD 2002-08-23
//-----------------------------------------------------------------------------
void MOS_OrderDialog_ABC::Cleanup()
{
    MOS_AgentDialog_ABC::Cleanup();    
}

//-----------------------------------------------------------------------------
// Name: MOS_OrderDialog_ABC::InitLimitComboBox
// Created: NLD 2002-08-22
//-----------------------------------------------------------------------------
void MOS_OrderDialog_ABC::InitLimitComboBox( QComboBox& comboBox )
{
    comboBox.clear();
    comboBox.insertItem( "-1" );
    MOS_LineManager::CT_LimitSet limitSet = MOS_App::GetApp().GetLineManager().GetLimitList();
    for( MOS_LineManager::CIT_LimitSet itLimit = limitSet.begin(); itLimit != limitSet.end(); ++itLimit )
        comboBox.insertItem( MT_FormatString( "%d", (*itLimit)->GetID() ).c_str() );
}

//-----------------------------------------------------------------------------
// Name: MOS_OrderDialog_ABC::InitLimaComboBox
// Created: NLD 2002-08-22
//-----------------------------------------------------------------------------
void MOS_OrderDialog_ABC::InitLimaComboBox( QComboBox& comboBox )
{
    comboBox.clear();
    MOS_LineManager::CT_LimaSet LimaSet = MOS_App::GetApp().GetLineManager().GetLimaList();
    for( MOS_LineManager::CIT_LimaSet itLima = LimaSet.begin(); itLima != LimaSet.end(); ++itLima )
        comboBox.insertItem( MT_FormatString( "%d", (*itLima)->GetID() ).c_str() );

//    comboBox.insertItem( "1" );
//    comboBox.insertItem( "2" );
//    comboBox.insertItem( "3" );
}

//-----------------------------------------------------------------------------
// Name: MOS_OrderDialog_ABC::InitLimaFunctionComboBox
// Created: NLD 2002-08-22
//-----------------------------------------------------------------------------
void MOS_OrderDialog_ABC::InitLimaFunctionComboBox( QComboBox& comboBox )
{
    comboBox.clear();
    comboBox.insertItem( "CR" );
    comboBox.insertItem( "Coord" );
    comboBox.insertItem( "Attitude change" );
    comboBox.insertItem( "Time coord" );
}


//=============================================================================
// SLOTS
//=============================================================================

//-----------------------------------------------------------------------------
// Name: MOS_OrderDialog_ABC::SlotValidate
// Created:  NLD 2002-01-03 
//-----------------------------------------------------------------------------
void MOS_OrderDialog_ABC::SlotValidate()
{
    //DIN_BufferedMessage msg = WriteOrder();

//    WriteOrderMsg( msg );

//    pAgent_->SendOrderMsg( msg );

    accept();
  
    Cleanup();
}


//-----------------------------------------------------------------------------
// Name: MOS_OrderDialog_ABC::SlotCancel
// Created:  NLD 2002-01-03 
//-----------------------------------------------------------------------------
void MOS_OrderDialog_ABC::SlotCancel()
{
    Cleanup();
    reject();
}


