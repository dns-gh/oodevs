//*****************************************************************************
// 
// $Created: AGN 03-07-21 $
// $Archive: /MVW_v10/Build/SDK/MOS_Light/src/MOS_LogMedicalConsign_ListView_Item.cpp $
// $Author: Nld $
// $Modtime: 14/01/05 17:06 $
// $Revision: 1 $
// $Workfile: MOS_LogMedicalConsign_ListView_Item.cpp $
// 
//*****************************************************************************

#include "MOS_Light_Pch.h"
#include "MOS_LogMedicalConsign_ListView_Item.h"

#include "MOS_App.h"
#include "MOS_LogMedicalConsign.h"
#include "MOS_Agent.h"

// -----------------------------------------------------------------------------
// Name: MOS_LogMedicalConsign_ListView_Item constructor
// Created: NLD 2004-12-30
// -----------------------------------------------------------------------------
MOS_LogMedicalConsign_ListView_Item::MOS_LogMedicalConsign_ListView_Item( QListView* pParent, MOS_LogMedicalConsign& consign )
    : QListViewItem                ( pParent, "Fusse" )
    , consign_                     ( consign )
    , pListViewItemPion_           ( 0 )
    , pListViewItemLogPionHandling_( 0 )
    , pListViewItemWound_          ( 0 )
    , pListViewItemMental_         ( 0 )
    , pListViewItemNBC_            ( 0 )
    , pListViewItemState_          ( 0 )
{
    // Label
    std::string strLabel( "Consign " );
    strLabel += itostring( consign_.GetID() );
    setText( 0, strLabel.c_str() );

    pListViewItemPion_            = new QListViewItem( this                               , MT_FormatString( "ID Pion demandeur : %s", consign.GetPion().GetName().c_str() ).c_str() );
    pListViewItemLogPionHandling_ = new QListViewItem( this, pListViewItemPion_           , "Pas de pion log traitant la consigne" );
    pListViewItemWound_           = new QListViewItem( this, pListViewItemLogPionHandling_, MT_FormatString( "Blessure : %s\n", consign_.GetWoundAsString().c_str() ).c_str() );
    pListViewItemMental_          = new QListViewItem( this, pListViewItemWound_          , consign_.IsMentalDeceased() ? "Reac mental" : "Non reac mental" );
    pListViewItemNBC_             = new QListViewItem( this, pListViewItemMental_         , consign_.IsContaminated() ? "Contaminé NBC" : "Non contaminé NBC" );
    pListViewItemState_           = new QListViewItem( this, pListViewItemNBC_            , "" );

    Update();

    consign_.AddListViewItem( *this );

    setOpen( true );
}

// -----------------------------------------------------------------------------
// Name: MOS_LogMedicalConsign_ListView_Item destructor
// Created: NLD 2004-12-30
// -----------------------------------------------------------------------------
MOS_LogMedicalConsign_ListView_Item::~MOS_LogMedicalConsign_ListView_Item()
{
    consign_.RemoveListViewItem( *this );
}

//-----------------------------------------------------------------------------
// Name: MOS_LogMedicalConsign_ListView_Item::Update
// Created: AGN 03-07-29*
//-----------------------------------------------------------------------------
void MOS_LogMedicalConsign_ListView_Item::Update()
{
    if( consign_.GetPionLogHandling() )
        pListViewItemLogPionHandling_->setText( 0, MT_FormatString( "Pion log traitant consigne : %s", consign_.GetPionLogHandling()->GetName().c_str() ).c_str() );
    else
        pListViewItemLogPionHandling_->setText( 0, "Pas de pion log traitant la consigne" );

    MOS_LogMedicalConsign::E_State nState = consign_.GetState();

    std::string strState( "Fussoir" );
    switch( nState ) 
    {        
        case MOS_LogMedicalConsign::eWaitingForEvacuation               : strState = "WaitingForEvacuation           "; break;
        case MOS_LogMedicalConsign::eEvacuationGoingTo                  : strState = "EvacuationGoingTo              "; break;
        case MOS_LogMedicalConsign::eEvacuationLoading                  : strState = "EvacuationLoading              "; break;
        case MOS_LogMedicalConsign::eEvacuationWaitingForFullLoading    : strState = "EvacuationWaitingForFullLoading"; break;
        case MOS_LogMedicalConsign::eEvacuationGoingFrom                : strState = "EvacuationGoingFrom            "; break;
        case MOS_LogMedicalConsign::eEvacuationUnloading                : strState = "EvacuationUnloading            "; break;
        case MOS_LogMedicalConsign::eWaitingForDiagnostic               : strState = "WaitingForDiagnostic           "; break;
        case MOS_LogMedicalConsign::eDiagnosing                         : strState = "Diagnosing                     "; break;
        case MOS_LogMedicalConsign::eSearchingForSortingArea            : strState = "SearchingForSortingArea        "; break;
        case MOS_LogMedicalConsign::eWaitingForSorting                  : strState = "WaitingForSorting              "; break;
        case MOS_LogMedicalConsign::eSorting                            : strState = "Sorting                        "; break;
        case MOS_LogMedicalConsign::eSearchingForHealingArea            : strState = "SearchingForHealingArea        "; break;
        case MOS_LogMedicalConsign::eWaitingForHealing                  : strState = "WaitingForHealing              "; break;
        case MOS_LogMedicalConsign::eHealing                            : strState = "Healing                        "; break;
        case MOS_LogMedicalConsign::eWaitingForCollection               : strState = "WaitingForCollection           "; break;
        case MOS_LogMedicalConsign::eCollectionLoading                  : strState = "CollectionLoading              "; break;
        case MOS_LogMedicalConsign::eCollectionWaitingForFullLoading    : strState = "CollectionWaitingForFullLoading"; break;
        case MOS_LogMedicalConsign::eCollectionGoingTo                  : strState = "CollectionGoingTo              "; break;
        case MOS_LogMedicalConsign::eCollectionUnloading                : strState = "CollectionUnloading            "; break;
        case MOS_LogMedicalConsign::eFinished                           : strState = "Finished                       "; break;
        default:
            assert( false );
    }
    pListViewItemState_->setText( 0, MT_FormatString( "Etat : %s\n", strState.c_str() ).c_str() );

    pListViewItemWound_->setText ( 0, MT_FormatString( "Blessure : %s\n", consign_.GetWoundAsString().c_str() ).c_str() );
    pListViewItemMental_->setText( 0, consign_.IsMentalDeceased() ? "Reac mental" : "Non reac mental" );
    pListViewItemNBC_->setText   ( 0, consign_.IsContaminated() ? "Contaminé NBC" : "Non contaminé NBC" );
}

