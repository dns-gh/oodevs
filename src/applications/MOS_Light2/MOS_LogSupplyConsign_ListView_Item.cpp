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
    , pListViewItemLogPionHandling_( 0 )
    , pListViewItemState_          ( 0 )
    , pListViewItemDotations_      ( 0 )
{
    // Label
    setText( 0, tr( "Consigne " ) + itostring( pConsign_->GetID() ).c_str() );

    pListViewItemPion_             = new QListViewItem( this                                , tr( "ID Automate demandeur : " ) + consign.GetPion().GetName().c_str() );
    pListViewItemLogPionHandling_  = new QListViewItem( this, pListViewItemPion_            , tr( "Pas de pion log. traitant la consigne" ) );
    pListViewItemLogPionConvoying_ = new QListViewItem( this, pListViewItemLogPionHandling_ , tr( "Pas de pion log. convoyant la consigne" ) );
    pListViewItemState_            = new QListViewItem( this, pListViewItemLogPionConvoying_, "" );
    pListViewItemDotations_        = new QListViewItem( this, pListViewItemState_           , tr( "Dotations demandées/accordées" ) );

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
    if( pConsign_->GetPionLogHandling() )
        pListViewItemLogPionHandling_->setText( 0, tr( "Pion log. traitant la consigne : " ) + pConsign_->GetPionLogHandling()->GetName().c_str() );
    else
        pListViewItemLogPionHandling_->setText( 0, tr( "Pas de pion log. traitant la consigne" ) );

    if( pConsign_->GetPionLogConvoying() )
        pListViewItemLogPionConvoying_->setText( 0, tr( "Pion log. convoyant la consigne : " ) + pConsign_->GetPionLogConvoying()->GetName().c_str() );
    else
        pListViewItemLogPionConvoying_->setText( 0, tr( "Pas de pion log. convoyant la consigne" ) );

    MOS_LogSupplyConsign::E_State nState = pConsign_->GetState();

    QString strState( tr( "Aucun" ) );

    switch( nState ) 
    {     
        case MOS_LogSupplyConsign::eConvoyForming    : strState = tr( "convoi constitution" ); break;
        case MOS_LogSupplyConsign::eConvoyLoading    : strState = tr( "convoi chargement" ); break;
        case MOS_LogSupplyConsign::eConvoyGoingTo    : strState = tr( "convoi deplacement aller" ); break;
        case MOS_LogSupplyConsign::eConvoyUnloading  : strState = tr( "Convoi déchargement" ); break;
        case MOS_LogSupplyConsign::eConvoyGoingFrom  : strState = tr( "Convoi deplacement retour" ); break;
        case MOS_LogSupplyConsign::eFinished         : strState = tr( "terminé" ); break;
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
