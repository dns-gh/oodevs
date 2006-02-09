//****************************************************************************
//
// $Created:  NLD 2002-01-03 $
// $Archive: /MVW_v10/Build/SDK/Light2/src/LogisticSupplyChangeQuotasDialog.cpp $
// $Author: Age $
// $Modtime: 6/04/05 17:35 $
// $Revision: 1 $
// $Workfile: LogisticSupplyChangeQuotasDialog.cpp $
//
//*****************************************************************************

#include "astec_pch.h"

#include "LogisticSupplyChangeQuotasDialog.h"
#include "moc_LogisticSupplyChangeQuotasDialog.cpp"

#include "App.h"
#include "ASN_Messages.h"
#include "Attr_Def.h"
#include "Agent.h"
#include "AgentManager.h"

// -----------------------------------------------------------------------------
// Name: LogisticSupplyChangeQuotasDialog constructor
// Created: NLD 2004-10-25
// -----------------------------------------------------------------------------
LogisticSupplyChangeQuotasDialog::LogisticSupplyChangeQuotasDialog( QWidget* pParent  )
    : QDialog ( pParent, "Affecter quotas rav" )
    , pAgent_ ( 0 )
    , pQuotas_( 0 )
    , pTypesMenu_  ( 0 )
    , pSuppliedComboBox_( 0 )
{
//    resize( 400, 150 );
    QVBoxLayout* pMainLayout = new QVBoxLayout( this );

    pSuppliedComboBox_ = new QComboBox( FALSE, this );
    pMainLayout->addWidget( pSuppliedComboBox_ );

    pQuotas_ = new QListView( this, "" );
    pQuotas_->addColumn( tr( "Dotation" ) );
    pQuotas_->addColumn( tr( "Quantité" ) );
    pQuotas_->setSorting( -1 );
    pMainLayout->addWidget( pQuotas_ );

    QHBoxLayout* pButtonLayout = new QHBoxLayout( pMainLayout );
    QPushButton* pCancelButton = new QPushButton( tr("Annuler"), this );
    QPushButton* pOKButton     = new QPushButton( tr("OK")    , this );
    pButtonLayout->addWidget( pCancelButton );
    pButtonLayout->addWidget( pOKButton     );
    pOKButton->setDefault( TRUE );

    connect( pCancelButton, SIGNAL( clicked() ), SLOT( Reject() ) );
    connect( pOKButton    , SIGNAL( clicked() ), SLOT( Validate() ) );

    pTypesMenu_ = new QPopupMenu( this );

    connect( pQuotas_          , SIGNAL( contextMenuRequested( QListViewItem*, const QPoint &, int ) ), SLOT(OnContextMenu( QListViewItem*, const QPoint&, int )) );
    connect( pQuotas_          , SIGNAL( itemRenamed( QListViewItem*, int, const QString& ) ), SLOT( onItemRenamed( QListViewItem*, int, const QString& ) ) );
    connect( pSuppliedComboBox_, SIGNAL( activated( int ) ), SLOT( OnSuppliedChanged( int ) ) );
}

// -----------------------------------------------------------------------------
// Name: LogisticSupplyChangeQuotasDialog destructor
// Created: NLD 2004-10-25
// -----------------------------------------------------------------------------
LogisticSupplyChangeQuotasDialog::~LogisticSupplyChangeQuotasDialog()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: LogisticSupplyChangeQuotasDialog::onItemRenamed
// Created: HME 2005-10-14
// -----------------------------------------------------------------------------
void LogisticSupplyChangeQuotasDialog::onItemRenamed( QListViewItem* /*item*/, int /*col*/, const QString& /*text*/ )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: LogisticSupplyChangeQuotasDialog::SetAgent
// Created: NLD 2004-11-30
// -----------------------------------------------------------------------------
void LogisticSupplyChangeQuotasDialog::SetAgent( const Agent& agent )
{
    pAgent_ = &agent;

    pQuotas_->clear();
    pTypesMenu_->clear();
    if( ! agent.IsLogisticRavitaillement() )
        return;

    automateComboBoxIDs_.clear();
    pSuppliedComboBox_->clear();

    AgentManager::CT_AgentMap& agents = App::GetApp().GetAgentManager().GetAgentList();
    for( AgentManager::CIT_AgentMap itAgent = agents.begin(); itAgent != agents.end(); ++itAgent )
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
// Name: LogisticSupplyChangeQuotasDialog::Validate
// Created: NLD 2004-10-25
// -----------------------------------------------------------------------------
void LogisticSupplyChangeQuotasDialog::Validate()
{
    assert( pAgent_ );

    CIT_AgentIDMap itSupplied = automateComboBoxIDs_.find( pSuppliedComboBox_->currentItem() );
    if ( itSupplied == automateComboBoxIDs_.end() )
        return;

    const Agent& supplied = *itSupplied->second;
    
    if ( App::GetApp().IsODBEdition() )
    {
        if ( pQuotas_->childCount() > 0 )
        {
            Agent::T_LogisticAvailabilities& quotas = supplied.pSupplyData_->quotas_;
            quotas.clear();
            QListViewItem* pItem = pQuotas_->firstChild();
            while( pItem )
            {
                quotas.push_back( std::pair< uint, uint >(App::GetApp().GetRessourceID( (const char*)pItem->text( 0 ) ) , pItem->text( 1 ).toInt()) );
                pItem = pItem->nextSibling();
            }
        }
        hide();
        return;
    }
    
    ASN_MsgLogRavitaillementChangeQuotas asnMsg;

    asnMsg.GetAsnMsg().oid_donneur  = pAgent_->GetID();
    asnMsg.GetAsnMsg().oid_receveur = supplied.GetID();

    asnMsg.GetAsnMsg().quotas.n = pQuotas_->childCount();
    if( asnMsg.GetAsnMsg().quotas.n > 0 )
    {
        ASN1T_DotationQuota* pAsnQuota = new ASN1T_DotationQuota[ asnMsg.GetAsnMsg().quotas.n ];
        QListViewItem* pItem = pQuotas_->firstChild();
        uint i = 0;
        while( pItem )
        {
            pAsnQuota[i].ressource_id     = App::GetApp().GetRessourceID( (const char*)pItem->text( 0 ) );
            pAsnQuota[i].quota_disponible = pItem->text( 1 ).toInt();
            ++i;
            pItem = pItem->nextSibling();
        }

        asnMsg.GetAsnMsg().quotas.elem = pAsnQuota;
    }

    asnMsg.Send( 6 );
    if( asnMsg.GetAsnMsg().quotas.n > 0 )
        delete [] asnMsg.GetAsnMsg().quotas.elem;

    hide();
}

// -----------------------------------------------------------------------------
// Name: LogisticSupplyChangeQuotasDialog::Reject
// Created: NLD 2004-10-25
// -----------------------------------------------------------------------------
void LogisticSupplyChangeQuotasDialog::Reject()
{
    pAgent_ = 0;
    hide();
}

// -----------------------------------------------------------------------------
// Name: LogisticSupplyChangeQuotasDialog::OnContextMenu
// Created: NLD 2005-02-03
// -----------------------------------------------------------------------------
void LogisticSupplyChangeQuotasDialog::OnContextMenu( QListViewItem* pItem, const QPoint& pos, int )
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
        QListViewItem* pItem = new QListViewItem( pQuotas_, pQuotas_->lastItem(), pTypesMenu_->text( nResult ), "10" );
        pItem->setRenameEnabled( 1, true );
    }
}

// -----------------------------------------------------------------------------
// Name: LogisticSupplyChangeQuotasDialog::OnSuppliedChanged
// Created: SBO 2005-10-27
// -----------------------------------------------------------------------------
void LogisticSupplyChangeQuotasDialog::OnSuppliedChanged( int nItem )
{
    CIT_AgentIDMap itSupplied = automateComboBoxIDs_.find( nItem );
    assert( itSupplied != automateComboBoxIDs_.end() );

    const Agent& supplied = *itSupplied->second;

    std::set< std::string > menuRessources;

    pTypesMenu_->clear();
    const T_ResourceQty_Map& ressources = supplied.pSupplyData_->stocks_;
    for( CIT_ResourceQty_Map it = ressources.begin(); it != ressources.end(); ++it )
    {
        const std::string strRessourceName = App::GetApp().GetResourceName( it->first );
        pTypesMenu_->insertItem( strRessourceName.c_str(), it->first );
        menuRessources.insert( strRessourceName );
    }

    const Agent::T_AgentVector& childs = supplied.children_;
    for( Agent::CIT_AgentVector itChild = childs.begin(); itChild != childs.end(); ++itChild )
    {
        if( !( *itChild )->pSupplyData_ )
            continue;
        const T_ResourceQty_Map& ressources = ( *itChild )->pSupplyData_->stocks_;
        for( CIT_ResourceQty_Map it = ressources.begin(); it != ressources.end(); ++it )
        {
            const std::string strRessourceName = App::GetApp().GetResourceName( it->first );
            if( menuRessources.find( strRessourceName ) == menuRessources.end() )
            {
                pTypesMenu_->insertItem( strRessourceName.c_str(), it->first );
                menuRessources.insert( strRessourceName );
            }
        }
    }

    pQuotas_->clear();
    const Agent::T_LogisticAvailabilities quotas = supplied.pSupplyData_->quotas_;
    for ( Agent::CIT_LogisticAvailabilities it = quotas.begin(); it != quotas.end(); ++it )
    {
        std::string str = App::GetApp().GetResourceName( it->first );
        int value = it->second;
        QListViewItem* item = new QListViewItem( pQuotas_, str.c_str(), QString::number( value ) );
        item->setRenameEnabled( 1, true );
    }
    pQuotas_->show();
}
