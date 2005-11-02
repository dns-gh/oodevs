//****************************************************************************
//
// $Created:  NLD 2002-01-03 $
// $Archive: /MVW_v10/Build/SDK/MOS_Light2/src/MOS_LogisticSupplyPushFlowDialog.cpp $
// $Author: Age $
// $Modtime: 6/04/05 17:35 $
// $Revision: 2 $
// $Workfile: MOS_LogisticSupplyPushFlowDialog.cpp $
//
//*****************************************************************************

#include "MOS_Light2_pch.h"

#include "MOS_LogisticSupplyPushFlowDialog.h"
#include "moc_MOS_LogisticSupplyPushFlowDialog.cpp"

#include "MOS_App.h"
#include "MOS_ASN_Messages.h"
#include "MOS_Attr_Def.h"
#include "MOS_Agent.h"
#include "MOS_AgentManager.h"

// -----------------------------------------------------------------------------
// Name: MOS_LogisticSupplyPushFlowDialog constructor
// Created: NLD 2004-10-25
// -----------------------------------------------------------------------------
MOS_LogisticSupplyPushFlowDialog::MOS_LogisticSupplyPushFlowDialog( QWidget* pParent  )
    : QDialog           ( pParent, "Pousser flux" )
    , pAgent_           ( 0 )
    , pStocks_          ( 0 )
    , pTypesMenu_       ( 0 )
    , pSuppliedComboBox_( 0 )
    , bInitialized_     ( false )
{
//    resize( 400, 150 );
    setCaption( tr( "Pousser flux" ) );
    QVBoxLayout* pMainLayout = new QVBoxLayout( this );

    pSuppliedComboBox_ = new QComboBox( FALSE, this );
    pMainLayout->addWidget( pSuppliedComboBox_ );

    pStocks_ = new QListView( this, "" );
    pStocks_->addColumn( tr( "Dotation" ) );
    pStocks_->addColumn( tr( "Quantité" ) );
    pStocks_->setSorting( -1 );
    pStocks_->setDefaultRenameAction( QListView::Accept );
    pMainLayout->addWidget( pStocks_ );
   
    QHBoxLayout* pButtonLayout = new QHBoxLayout( pMainLayout );
    QPushButton* pCancelButton = new QPushButton( tr("Annuler"), this );
    QPushButton* pOKButton     = new QPushButton( tr("OK")    , this );
    pButtonLayout->addWidget( pCancelButton );
    pButtonLayout->addWidget( pOKButton     );
    pOKButton->setDefault( TRUE );

    connect( pCancelButton, SIGNAL( clicked() ), SLOT( Reject() ) );
    connect( pOKButton    , SIGNAL( clicked() ), SLOT( Validate() ) );    

    pTypesMenu_ = new QPopupMenu( this );

    connect( pStocks_, SIGNAL( contextMenuRequested( QListViewItem*, const QPoint &, int ) ), SLOT(OnContextMenu( QListViewItem*, const QPoint&, int )) );
    connect( pSuppliedComboBox_, SIGNAL( activated( int ) ), SLOT( OnSuppliedChanged( int ) ) );
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
    pAgent_ = &agent;

    pStocks_->clear();
    pTypesMenu_->clear();
    if( ! agent.pSupplyData_ )
        return;

    automateComboBoxIDs_.clear();
    pSuppliedComboBox_->clear();

    MOS_AgentManager::CT_AgentMap& agents = MOS_App::GetApp().GetAgentManager().GetAgentList();
    for( MOS_AgentManager::CIT_AgentMap itAgent = agents.begin(); itAgent != agents.end(); ++itAgent )
    {
        if( itAgent->second && itAgent->second->nLogSupplySuperior_== agent.GetID() )
        {
            automateComboBoxIDs_.insert( std::make_pair( pSuppliedComboBox_->count(), itAgent->second ) );
            pSuppliedComboBox_->insertItem( itAgent->second->GetName().c_str() );
        }
    }
    if( pSuppliedComboBox_->count() > 0 )
        OnSuppliedChanged( pSuppliedComboBox_->currentItem() );
}

// -----------------------------------------------------------------------------
// Name: MOS_LogisticSupplyPushFlowDialog::Validate
// Created: NLD 2004-10-25
// -----------------------------------------------------------------------------
void MOS_LogisticSupplyPushFlowDialog::Validate()
{
    assert( pAgent_ );

    MOS_ASN_MsgLogRavitaillementPousserFlux asnMsg;

    asnMsg.GetAsnMsg().oid_donneur  = pAgent_->GetID();
    asnMsg.GetAsnMsg().oid_receveur = automateComboBoxIDs_.find( pSuppliedComboBox_->currentItem() )->second->GetID();

    asnMsg.GetAsnMsg().stocks.n = pStocks_->childCount();
    if( asnMsg.GetAsnMsg().stocks.n > 0 )
    {
        ASN1T_DotationStock* pAsnStock = new ASN1T_DotationStock[ asnMsg.GetAsnMsg().stocks.n ];
        QListViewItem* pItem = pStocks_->firstChild();
        uint i = 0;
        while( pItem )
        {
            pAsnStock[i].ressource_id        = MOS_App::GetApp().GetRessourceID( pItem->text( 0 ).ascii() );
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
    static QPopupMenu* pMenu  = new QPopupMenu( this );

    pMenu->clear();
    pMenu->insertItem( tr( "Ajouter dotation" ), pTypesMenu_ );

    if( pItem != 0 )
        pMenu->insertItem( tr( "Effacer dotation" ), 0 );

    int nResult =pMenu->exec( pos );
    if( nResult == -1 )
        return;

    if( nResult == 0 )
    {
        assert( pItem != 0 );
        delete pItem;
    }
    else
    {
        QListViewItem* pItem = new QListViewItem( pStocks_, pStocks_->lastItem(), pTypesMenu_->text( nResult ), "1" );
        pItem->setRenameEnabled( 1, true );
    }
}

// -----------------------------------------------------------------------------
// Name: MOS_LogisticSupplyPushFlowDialog::OnSuppliedChanged
// Created: SBO 2005-10-27
// -----------------------------------------------------------------------------
void MOS_LogisticSupplyPushFlowDialog::OnSuppliedChanged( int nItem )
{
    CIT_AgentIDMap itSupplied = automateComboBoxIDs_.find( nItem );
    assert( itSupplied != automateComboBoxIDs_.end() );

    const MOS_Agent& supplied = *itSupplied->second;

    std::set< std::string > menuRessources;

    pTypesMenu_->clear();
    const T_ResourceQty_Map& ressources = supplied.pSupplyData_->stocks_;
    for( CIT_ResourceQty_Map it = ressources.begin(); it != ressources.end(); ++it )
    {
        const std::string strRessourceName = MOS_App::GetApp().GetResourceName( it->first );
        pTypesMenu_->insertItem( strRessourceName.c_str(), it->first );
        menuRessources.insert( strRessourceName );
    }

    const MOS_Agent::T_AgentVector& childs = supplied.children_;
    for( MOS_Agent::CIT_AgentVector itChild = childs.begin(); itChild != childs.end(); ++itChild )
    {
        if( !( *itChild )->pSupplyData_ )
            continue;
        const T_ResourceQty_Map& ressources = ( *itChild )->pSupplyData_->stocks_;
        for( CIT_ResourceQty_Map it = ressources.begin(); it != ressources.end(); ++it )
        {
            const std::string strRessourceName = MOS_App::GetApp().GetResourceName( it->first );
            if( menuRessources.find( strRessourceName ) == menuRessources.end() )
            {
                pTypesMenu_->insertItem( strRessourceName.c_str(), it->first );
                menuRessources.insert( strRessourceName );
            }
        }
    }
}