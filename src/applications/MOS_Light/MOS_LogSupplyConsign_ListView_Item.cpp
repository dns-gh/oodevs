//*****************************************************************************
// 
// $Created: AGN 03-07-21 $
// $Archive: /MVW_v10/Build/SDK/MOS_Light/src/MOS_LogSupplyConsign_ListView_Item.cpp $
// $Author: Nld $
// $Modtime: 10/02/05 16:06 $
// $Revision: 2 $
// $Workfile: MOS_LogSupplyConsign_ListView_Item.cpp $
// 
//*****************************************************************************

#include "MOS_Light_pch.h"
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
    , consign_                     ( consign )
    , pListViewItemPion_           ( 0 )
    , pListViewItemLogPionHandling_( 0 )
    , pListViewItemState_          ( 0 )
    , pListViewItemDotations_      ( 0 )
{
    // Label
    std::string strLabel( "Consign " );
    strLabel += itostring( consign_.GetID() );
    setText( 0, strLabel.c_str() );

    pListViewItemPion_             = new QListViewItem( this                                , MT_FormatString( "ID Automate demandeur : %s", consign.GetPion().GetName().c_str() ).c_str() );
    pListViewItemLogPionHandling_  = new QListViewItem( this, pListViewItemPion_            , "Pas de pion log traitant la consigne" );
    pListViewItemLogPionConvoying_ = new QListViewItem( this, pListViewItemLogPionHandling_ , "Pas de pion log convoyant la consigne" );
    pListViewItemState_            = new QListViewItem( this, pListViewItemLogPionConvoying_, "" );
    pListViewItemDotations_        = new QListViewItem( this, pListViewItemState_           , "Dotations demandées/accordées" );

    Update();
    consign_.AddListViewItem( *this );
    setOpen( true );
}

// -----------------------------------------------------------------------------
// Name: MOS_LogSupplyConsign_ListView_Item destructor
// Created: NLD 2004-12-30
// -----------------------------------------------------------------------------
MOS_LogSupplyConsign_ListView_Item::~MOS_LogSupplyConsign_ListView_Item()
{
    consign_.RemoveListViewItem( *this );
}

//-----------------------------------------------------------------------------
// Name: MOS_LogSupplyConsign_ListView_Item::Update
// Created: AGN 03-07-29*
//-----------------------------------------------------------------------------
void MOS_LogSupplyConsign_ListView_Item::Update()
{
    if( consign_.GetPionLogHandling() )
        pListViewItemLogPionHandling_->setText( 0, MT_FormatString( "Pion log traitant consigne : %s", consign_.GetPionLogHandling()->GetName().c_str() ).c_str() );
    else
        pListViewItemLogPionHandling_->setText( 0, "Pas de pion log traitant la consigne" );

    if( consign_.GetPionLogConvoying() )
        pListViewItemLogPionConvoying_->setText( 0, MT_FormatString( "Pion log convoyant consigne : %s", consign_.GetPionLogConvoying()->GetName().c_str() ).c_str() );
    else
        pListViewItemLogPionConvoying_->setText( 0, "Pas de pion log convoyant la consigne" );

    MOS_LogSupplyConsign::E_State nState = consign_.GetState();

    std::string strState( "Fussoir" );
    switch( nState ) 
    {     
        case MOS_LogSupplyConsign::eConvoyForming    : strState = "ConvoyForming"; break;
        case MOS_LogSupplyConsign::eConvoyLoading    : strState = "ConvoyLoading"; break;
        case MOS_LogSupplyConsign::eConvoyGoingTo    : strState = "ConvoyGoingTo"; break;
        case MOS_LogSupplyConsign::eConvoyUnloading  : strState = "ConvoyUnloading"; break;
        case MOS_LogSupplyConsign::eConvoyGoingFrom  : strState = "ConvoyGoingFrom"; break;
        case MOS_LogSupplyConsign::eFinished         : strState = "Finished"; break;
        default:
            assert( false );
    }
    pListViewItemState_->setText( 0, MT_FormatString( "Etat : %s\n", strState.c_str() ).c_str() );

    delete pListViewItemDotations_;
    pListViewItemDotations_ = new QListViewItem( this, pListViewItemState_, "Dotations demandées/accordées/convoyées" );
    for( MOS_LogSupplyConsign::CIT_DotationMap itDot = consign_.GetDotations().begin(); itDot != consign_.GetDotations().end(); ++itDot )
        new QListViewItem( pListViewItemDotations_, MT_FormatString( "%s : %d/%d/%d", MOS_App::GetApp().GetRessourceName( itDot->first ).c_str(), itDot->second.nNbrRequested_, itDot->second.nNbrReserved_, itDot->second.nNbrConvoyed_).c_str() );
}

