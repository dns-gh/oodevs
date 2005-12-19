//*****************************************************************************
// 
// $Created: AGN 03-07-21 $
// $Archive: /MVW_v10/Build/SDK/MOS_Light2/src/MOS_LogSupplyConsign_ListView_Item.cpp $
// $Author: Age $
// $Modtime: 6/04/05 10:40 $
// $Revision: 2 $
// $Workfile: MOS_LogSupplyConsign_ListView_Item.cpp $
// 
//*****************************************************************************

#include "MOS_Light2_pch.h"
#include "MOS_LogSupplyConsign_ListView_Item.h"

#include "MOS_App.h"
#include "MOS_LogSupplyConsign.h"
#include "MOS_Agent.h"

// -----------------------------------------------------------------------------
// Name: MOS_LogSupplyConsign_ListView_Item constructor
// Created: NLD 2004-12-30
// -----------------------------------------------------------------------------
MOS_LogSupplyConsign_ListView_Item::MOS_LogSupplyConsign_ListView_Item( QListView* pParent, MOS_LogSupplyConsign& consign )
    : QListViewItem                ( pParent, "Fusse" )
    , pConsign_                    ( &consign )
    , pListViewItemPion_           ( 0 )
    , pListViewItemLogAutomateHandling_( 0 )
    , pListViewItemLogAutomateProvidingConvoyResources_( 0 )
    , pListViewItemState_          ( 0 )
    , pListViewItemDotations_      ( 0 )
{
    // Label
    setText( 0, tr( "Consigne " ) + itostring( pConsign_->GetID() ).c_str() );

    pListViewItemPion_                                = new QListViewItem( this                                                    , tr( "ID Automate demandeur : " ) + consign.GetPion().GetName().c_str() );
    pListViewItemLogAutomateHandling_                 = new QListViewItem( this, pListViewItemPion_                                , tr( "Pas d'automate log traitant la consigne" ) );
    pListViewItemLogAutomateProvidingConvoyResources_ = new QListViewItem( this, pListViewItemLogAutomateHandling_                 , tr( "Pas d'automate log fournissant les moyens (camions/chef) du convoi" ) );
    pListViewItemLogPionConvoying_                    = new QListViewItem( this, pListViewItemLogAutomateProvidingConvoyResources_ , tr( "Pas de pion convoi" ) );
    pListViewItemState_                               = new QListViewItem( this, pListViewItemLogPionConvoying_                    , "" );
    pListViewItemDotations_                           = new QListViewItem( this, pListViewItemState_                               , tr( "Dotations demandées/accordées" ) );

    Update();
    setOpen( true );
}

// -----------------------------------------------------------------------------
// Name: MOS_LogSupplyConsign_ListView_Item destructor
// Created: NLD 2004-12-30
// -----------------------------------------------------------------------------
MOS_LogSupplyConsign_ListView_Item::~MOS_LogSupplyConsign_ListView_Item()
{
    // NOTHING
}

//-----------------------------------------------------------------------------
// Name: MOS_LogSupplyConsign_ListView_Item::Update
// Created: AGN 03-07-29*
//-----------------------------------------------------------------------------
void MOS_LogSupplyConsign_ListView_Item::Update()
{
    if( pConsign_->GetAutomateLogHandling() )
        pListViewItemLogAutomateHandling_->setText( 0, tr( "Automate traitant la consigne : " ) + pConsign_->GetAutomateLogHandling()->GetName().c_str() );
    else
        pListViewItemLogAutomateHandling_->setText( 0, tr( "Pas d'automate traitant la consigne" ) );

    if( pConsign_->GetPionLogConvoying() )
        pListViewItemLogPionConvoying_->setText( 0, tr( "Convoi : " ) + pConsign_->GetPionLogConvoying()->GetName().c_str() );
    else
        pListViewItemLogPionConvoying_->setText( 0, tr( "Pas de convoi" ) );

    if( pConsign_->GetAutomateLogProvidingConvoyResources() )
        pListViewItemLogAutomateProvidingConvoyResources_->setText( 0, tr( "Automate fournissant les moyens (camions/chef) du convoi : " ) + pConsign_->GetAutomateLogProvidingConvoyResources()->GetName().c_str() );
    else
        pListViewItemLogAutomateProvidingConvoyResources_->setText( 0, tr( "Pas d'automate fournissant les moyens (camions/chef) du convoi" ) );

    MOS_LogSupplyConsign::E_State nState = pConsign_->GetState();

    QString strState( tr( "Aucun" ) );

    switch( nState ) 
    {     
        case MOS_LogSupplyConsign::eConvoyWaitingForCommander    : strState = tr( "Convoi en attente d'un chef de convoi" ); break;
        case MOS_LogSupplyConsign::eConvoyWaitingForTransporters: strState = tr( "Convoi en attente de camions" ); break;
        case MOS_LogSupplyConsign::eConvoyForming    : strState = tr( "Convoi en cours de constitution" ); break;
        
        case MOS_LogSupplyConsign::eConvoyGoingToLoadingPoint: strState = tr( "Convoi en déplacement vers point de chargement" ); break;
        case MOS_LogSupplyConsign::eConvoyLoading    : strState = tr( "Convoi en cours de chargement" ); break;
        
        case MOS_LogSupplyConsign::eConvoyGoingToUnloadingPoint: strState = tr( "Convoi en déplacement vers point de déchargement" ); break;
        case MOS_LogSupplyConsign::eConvoyUnloading  : strState = tr( "Convoi en cours de déchargement" ); break;
        
        case MOS_LogSupplyConsign::eConvoyGoingBackToFormingPoint  : strState = tr( "Convoi en retour" ); break;
        case MOS_LogSupplyConsign::eFinished         : strState = tr( "Terminé" ); break;
        default:
            assert( false );
    }
    pListViewItemState_->setText( 0, tr( "Etat : " ) + strState );

    delete pListViewItemDotations_;
    pListViewItemDotations_ = new QListViewItem( this, pListViewItemState_, tr( "Dotations demandées/accordées/convoyées" ) );
    for( MOS_LogSupplyConsign::CIT_DotationMap itDot = pConsign_->GetDotations().begin(); itDot != pConsign_->GetDotations().end(); ++itDot )
    {
        std::stringstream streamItem;
        streamItem << MOS_App::GetApp().GetResourceName( itDot->first ) << " : "
                   << itDot->second.nNbrRequested_ << "/"
                   << itDot->second.nNbrReserved_ << "/"
                   << itDot->second.nNbrConvoyed_;
        new QListViewItem( pListViewItemDotations_, streamItem.str().c_str() );
    }
}

// -----------------------------------------------------------------------------
// Name: MOS_LogSupplyConsign_ListView_Item::tr
// Created: AGE 2005-04-01
// -----------------------------------------------------------------------------
QString MOS_LogSupplyConsign_ListView_Item::tr( const QString& str ) const
{
    return qApp->tr( str, "MOS_LogSupplyConsign_ListView_Item" );
}

// -----------------------------------------------------------------------------
// Name: MOS_LogSupplyConsign_ListView_Item::Assign
// Created: AGE 2005-04-06
// -----------------------------------------------------------------------------
void MOS_LogSupplyConsign_ListView_Item::Assign( MOS_LogSupplyConsign& consign )
{
    pConsign_ = &consign;
    Update();
}
