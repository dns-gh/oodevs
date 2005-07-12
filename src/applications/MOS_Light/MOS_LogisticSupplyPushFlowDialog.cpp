//****************************************************************************
//
// $Created:  NLD 2002-01-03 $
// $Archive: /MVW_v10/Build/SDK/MOS_Light/src/MOS_LogisticSupplyPushFlowDialog.cpp $
// $Author: Nld $
// $Modtime: 3/02/05 17:03 $
// $Revision: 2 $
// $Workfile: MOS_LogisticSupplyPushFlowDialog.cpp $
//
//*****************************************************************************

#include "MOS_Light_Pch.h"

#include "MOS_LogisticSupplyPushFlowDialog.h"
#include "moc_MOS_LogisticSupplyPushFlowDialog.cpp"

#include "MOS_App.h"
#include "MOS_ASN_Messages.h"
#include "MOS_Attr_Def.h"
#include "MOS_Agent.h"

#include <qlistview.h>

// -----------------------------------------------------------------------------
// Name: MOS_LogisticSupplyPushFlowDialog constructor
// Created: NLD 2004-10-25
// -----------------------------------------------------------------------------
MOS_LogisticSupplyPushFlowDialog::MOS_LogisticSupplyPushFlowDialog( QWidget* pParent  )
    : QDialog ( pParent, "Pousser flux" )
    , pAgent_ ( 0 )
    , pStocks_( 0 )
    , pTypesMenu_  ( 0 )
    , pAutomateChangedComboBox_( 0 )
    , bInitialized_( false )
{
//    resize( 400, 150 );
    QVBoxLayout* pMainLayout = new QVBoxLayout( this );

    pAutomateChangedComboBox_ = new QComboBox( FALSE, this );
    pMainLayout->addWidget( pAutomateChangedComboBox_ );

    pStocks_ = new QListView( this, "" );
    pStocks_->addColumn( "Type dotation" );
    pStocks_->addColumn( "Nb" );
    pStocks_->setSorting( -1 );
    pMainLayout->addWidget( pStocks_ );
   
    QHBoxLayout* pButtonLayout = new QHBoxLayout( pMainLayout );
    QPushButton* pCancelButton = new QPushButton( tr("Cancel"), this );
    QPushButton* pOKButton     = new QPushButton( tr("OK")    , this );
    pButtonLayout->addWidget( pCancelButton );
    pButtonLayout->addWidget( pOKButton     );
    pOKButton->setDefault( TRUE );

    connect( pCancelButton, SIGNAL( clicked() ), SLOT( Reject() ) );
    connect( pOKButton    , SIGNAL( clicked() ), SLOT( Validate() ) );    

    pTypesMenu_ = new QPopupMenu( this );
    
    const MOS_App::T_MosId_String_Map& ressources = MOS_App::GetApp().GetRessources();
    for( MOS_App::CIT_MosId_String_Map it = ressources.begin(); it != ressources.end(); ++it )
        pTypesMenu_->insertItem( it->second.c_str(), it->first );

    connect( pStocks_, SIGNAL( contextMenuRequested( QListViewItem*, const QPoint &, int ) ), SLOT(OnContextMenu( QListViewItem*, const QPoint&, int )) );
}

// -----------------------------------------------------------------------------
// Name: MOS_LogisticSupplyPushFlowDialog destructor
// Created: NLD 2004-10-25
// -----------------------------------------------------------------------------
MOS_LogisticSupplyPushFlowDialog::~MOS_LogisticSupplyPushFlowDialog()
{

}

// -----------------------------------------------------------------------------
// Name: MOS_LogisticSupplyPushFlowDialog::SetAgent
// Created: NLD 2004-11-30
// -----------------------------------------------------------------------------
void MOS_LogisticSupplyPushFlowDialog::SetAgent( const MOS_Agent& agent )
{
    assert( !pAgent_ );
    pAgent_ = &agent;
    if( !bInitialized_ )
    {
        MOS_AgentManager::CT_AgentMap& agents = MOS_App::GetApp().GetAgentManager().GetAgentList();
        for( MOS_AgentManager::CIT_AgentMap itAgent = agents.begin(); itAgent != agents.end(); ++itAgent )
        {
            const MOS_Agent& agent = *itAgent->second;
            if( !agent.IsAutomate() || !agent.IsSupplyPionLog() )
                continue;

            automateComboBoxIDs_.insert( std::make_pair( pAutomateChangedComboBox_->count(), &agent ) );
            pAutomateChangedComboBox_->insertItem( agent.GetName().c_str() );
        }
        bInitialized_ = true;
    }
}

// -----------------------------------------------------------------------------
// Name: MOS_LogisticSupplyPushFlowDialog::Validate
// Created: NLD 2004-10-25
// -----------------------------------------------------------------------------
void MOS_LogisticSupplyPushFlowDialog::Validate()
{
    assert( pAgent_ );

    MOS_ASN_MsgLogRavitaillementPousserFlux asnMsg;

    asnMsg.GetAsnMsg().oid_donneur  = pAgent_->GetAgentID();
    asnMsg.GetAsnMsg().oid_receveur = automateComboBoxIDs_.find( pAutomateChangedComboBox_->currentItem() )->second->GetAgentID();

    asnMsg.GetAsnMsg().stocks.n = pStocks_->childCount();
    if( asnMsg.GetAsnMsg().stocks.n > 0 )
    {
        ASN1T_DotationStock* pAsnStock = new ASN1T_DotationStock[ asnMsg.GetAsnMsg().stocks.n ];
        QListViewItem* pItem = pStocks_->firstChild();
        uint i = 0;
        while( pItem )
        {
            pAsnStock[i].ressource_id        = MOS_App::GetApp().GetRessourceID( (const char*)pItem->text( 0 ) );
            pAsnStock[i].quantite_disponible = pItem->text( 1 ).toInt();
            ++i;
            pItem = pItem->nextSibling();
        }

        asnMsg.GetAsnMsg().stocks.elem = pAsnStock;
    }

    asnMsg.Send( 6 );
    if( asnMsg.GetAsnMsg().stocks.n > 0 )
        delete [] asnMsg.GetAsnMsg().stocks.elem;

    hide();
}

// -----------------------------------------------------------------------------
// Name: MOS_LogisticSupplyPushFlowDialog::Reject
// Created: NLD 2004-10-25
// -----------------------------------------------------------------------------
void MOS_LogisticSupplyPushFlowDialog::Reject()
{
    pAgent_ = 0;
    hide();
}

// -----------------------------------------------------------------------------
// Name: MOS_LogisticSupplyPushFlowDialog::OnContextMenu
// Created: NLD 2005-02-03
// -----------------------------------------------------------------------------
void MOS_LogisticSupplyPushFlowDialog::OnContextMenu( QListViewItem* pItem, const QPoint& pos, int )
{
    QPopupMenu menu( this );
    menu.insertItem( "Ajouter type", pTypesMenu_ );

    if( pItem != 0 )
        menu.insertItem( "Effacer type", 0 );

    int nResult = menu.exec( pos );
    if( nResult == -1 )
        return;

    if( nResult == 0 )
    {
        assert( pItem != 0 );
        delete pItem;
    }
    else
    {
        QListViewItem* pItem = new QListViewItem( pStocks_, pStocks_->lastItem(), pTypesMenu_->text( nResult ), "10" );
        pItem->setRenameEnabled( 1, true );
    }
}
