//****************************************************************************
//
// $Created:  NLD 2002-01-03 $
// $Archive: /MVW_v10/Build/SDK/Light2/src/LogisticSupplyPushFlowDialog.cpp $
// $Author: Age $
// $Modtime: 6/04/05 17:35 $
// $Revision: 2 $
// $Workfile: LogisticSupplyPushFlowDialog.cpp $
//
//*****************************************************************************

#include "astec_pch.h"

#include "LogisticSupplyPushFlowDialog.h"
#include "moc_LogisticSupplyPushFlowDialog.cpp"

#include "App.h"
#include "ASN_Messages.h"
#include "Attr_Def.h"
#include "Agent.h"
#include "AgentManager.h"
#include "Model.h"

// -----------------------------------------------------------------------------
// Name: LogisticSupplyPushFlowDialog constructor
// Created: NLD 2004-10-25
// -----------------------------------------------------------------------------
LogisticSupplyPushFlowDialog::LogisticSupplyPushFlowDialog( QWidget* pParent  )
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
    pStocks_->addColumn( tr( "Quantit�" ) );
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
// Name: LogisticSupplyPushFlowDialog destructor
// Created: NLD 2004-10-25
// -----------------------------------------------------------------------------
LogisticSupplyPushFlowDialog::~LogisticSupplyPushFlowDialog()
{

}

namespace
{
    template< typename Container, typename Combo >
    struct SupplyFiller
    {
        SupplyFiller( const Agent& agent, Container& cont, Combo* combo ) 
            : agent_( agent ), cont_( cont ), combo_( combo ) {};
        void operator()( const Agent& o ) const
        {
            if( o.nLogSupplySuperior_ == agent_.GetID() )
            {
                cont_.insert( std::make_pair( combo_->count(), &o ) );
                combo_->insertItem( o.GetName().c_str() );
            }
        };
    private:
        SupplyFiller& operator=( const SupplyFiller& );
        mutable Container& cont_;
        mutable Combo* combo_;
        const Agent& agent_;
    };
    template< typename Container, typename Combo >
    SupplyFiller< Container, Combo > SupplyFill( const Agent& agent, Container& cont, Combo* combo )
    {
        return SupplyFiller< Container, Combo >( agent, cont, combo );
    }
}

// -----------------------------------------------------------------------------
// Name: LogisticSupplyPushFlowDialog::SetAgent
// Created: NLD 2004-11-30
// -----------------------------------------------------------------------------
void LogisticSupplyPushFlowDialog::SetAgent( const Agent& agent )
{
    pAgent_ = &agent;

    pStocks_->clear();
    pTypesMenu_->clear();
    if( ! agent.pSupplyData_ )
        return;

    automateComboBoxIDs_.clear();
    pSuppliedComboBox_->clear();

    App::GetApp().GetModel().ApplyOnAgents( SupplyFill( agent, automateComboBoxIDs_, pSuppliedComboBox_ ) );
    if( pSuppliedComboBox_->count() > 0 )
        OnSuppliedChanged( pSuppliedComboBox_->currentItem() );
}

// -----------------------------------------------------------------------------
// Name: LogisticSupplyPushFlowDialog::Validate
// Created: NLD 2004-10-25
// -----------------------------------------------------------------------------
void LogisticSupplyPushFlowDialog::Validate()
{
    assert( pAgent_ );

    ASN_MsgLogRavitaillementPousserFlux asnMsg;

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
            pAsnStock[i].ressource_id        = App::GetApp().GetRessourceID( pItem->text( 0 ).ascii() );
            pAsnStock[i].quantite_disponible = pItem->text( 1 ).toInt();
            ++i;
            pItem = pItem->nextSibling();
        }

        asnMsg.GetAsnMsg().stocks.elem = pAsnStock;
    }

    asnMsg.Send();
    if( asnMsg.GetAsnMsg().stocks.n > 0 )
        delete [] asnMsg.GetAsnMsg().stocks.elem;

    hide();
}

// -----------------------------------------------------------------------------
// Name: LogisticSupplyPushFlowDialog::Reject
// Created: NLD 2004-10-25
// -----------------------------------------------------------------------------
void LogisticSupplyPushFlowDialog::Reject()
{
    pAgent_ = 0;
    hide();
}

// -----------------------------------------------------------------------------
// Name: LogisticSupplyPushFlowDialog::OnContextMenu
// Created: NLD 2005-02-03
// -----------------------------------------------------------------------------
void LogisticSupplyPushFlowDialog::OnContextMenu( QListViewItem* pItem, const QPoint& pos, int )
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
// Name: LogisticSupplyPushFlowDialog::OnSuppliedChanged
// Created: SBO 2005-10-27
// -----------------------------------------------------------------------------
void LogisticSupplyPushFlowDialog::OnSuppliedChanged( int nItem )
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
}