//*****************************************************************************
// 
// $Created: AGN 03-07-21 $
// $Archive: /MVW_v10/Build/SDK/MOS_Light/src/MOS_LogMaintenanceConsign_ListView_Item.cpp $
// $Author: Nld $
// $Modtime: 14/01/05 17:06 $
// $Revision: 2 $
// $Workfile: MOS_LogMaintenanceConsign_ListView_Item.cpp $
// 
//*****************************************************************************

#include "MOS_Light_Pch.h"
#include "MOS_LogMaintenanceConsign_ListView_Item.h"

#include "MOS_App.h"
#include "MOS_LogMaintenanceConsign.h"
#include "MOS_Agent.h"

// -----------------------------------------------------------------------------
// Name: MOS_LogMaintenanceConsign_ListView_Item constructor
// Created: NLD 2004-12-30
// -----------------------------------------------------------------------------
MOS_LogMaintenanceConsign_ListView_Item::MOS_LogMaintenanceConsign_ListView_Item( QListView* pParent, MOS_LogMaintenanceConsign& consign )
    : QListViewItem                ( pParent, "Fusse" )
    , consign_                     ( consign )
    , pListViewItemPion_           ( 0 )
    , pListViewItemLogPionHandling_( 0 )
    , pListViewItemEquipmentType_  ( 0 )
    , pListViewItemBreakdownType_  ( 0 )
    , pListViewItemState_          ( 0 )
{
    // Label
    std::string strLabel( "Consign " );
    strLabel += itostring( consign_.GetID() );
    setText( 0, strLabel.c_str() );

    pListViewItemPion_            = new QListViewItem( this                                , MT_FormatString( "ID Pion demandeur : %s", consign.GetPion().GetName().c_str() ).c_str() );
    pListViewItemLogPionHandling_ = new QListViewItem( this, pListViewItemPion_            , "Pas de pion log traitant la consigne" );
    pListViewItemEquipmentType_   = new QListViewItem( this, pListViewItemLogPionHandling_ , MT_FormatString( "Type d'équipement : %s", MOS_App::GetApp().GetEquipementName( consign_.GetEquipmentTypeID() ).c_str() ).c_str() );
    pListViewItemBreakdownType_   = new QListViewItem( this, pListViewItemEquipmentType_   , MT_FormatString( "Type de panne : %s", MOS_App::GetApp().GetBreakDownName( consign_.GetBreakdownTypeID() ).c_str() ).c_str() );
    pListViewItemState_           = new QListViewItem( this, pListViewItemBreakdownType_   , "" );

    Update();

    consign_.AddListViewItem( *this );

    setOpen( true );
}

// -----------------------------------------------------------------------------
// Name: MOS_LogMaintenanceConsign_ListView_Item destructor
// Created: NLD 2004-12-30
// -----------------------------------------------------------------------------
MOS_LogMaintenanceConsign_ListView_Item::~MOS_LogMaintenanceConsign_ListView_Item()
{
    consign_.RemoveListViewItem( *this );
}

//-----------------------------------------------------------------------------
// Name: MOS_LogMaintenanceConsign_ListView_Item::Update
// Created: AGN 03-07-29*
//-----------------------------------------------------------------------------
void MOS_LogMaintenanceConsign_ListView_Item::Update()
{
    if( consign_.GetPionLogHandling() )
        pListViewItemLogPionHandling_->setText( 0, MT_FormatString( "Pion log traitant consigne : %s", consign_.GetPionLogHandling()->GetName().c_str() ).c_str() );
    else
        pListViewItemLogPionHandling_->setText( 0, "Pas de pion log traitant la consigne" );

    MOS_LogMaintenanceConsign::E_State nState = consign_.GetState();

    std::string strState( "Fussoir" );
    switch( nState ) 
    {        
        case MOS_LogMaintenanceConsign::eGoingFrom               : strState = "GoingFrom"; break;         
        case MOS_LogMaintenanceConsign::eWaitingForCarrier       : strState = "WaitingForCarrier"; break;         
        case MOS_LogMaintenanceConsign::eCarrierGoingTo          : strState = "CarrierGoingTo"; break;         
        case MOS_LogMaintenanceConsign::eCarrierLoading          : strState = "CarrierLoading"; break;         
        case MOS_LogMaintenanceConsign::eCarrierGoingFrom        : strState = "CarrierGoingFrom"; break;         
        case MOS_LogMaintenanceConsign::eCarrierUnloading        : strState = "CarrierUnloading"; break;         
        case MOS_LogMaintenanceConsign::eDiagnosing              : strState = "Diagnosing"; break;         
        case MOS_LogMaintenanceConsign::eSearchingForUpperLevel  : strState = "SearchingForUpperLevel"; break;         
        case MOS_LogMaintenanceConsign::eWaitingForParts         : strState = "WaitingForParts"; break;         
        case MOS_LogMaintenanceConsign::eWaitingForRepairer      : strState = "WaitingForRepairer"; break;         
        case MOS_LogMaintenanceConsign::eRepairing               : strState = "Repairing"; break;         
        case MOS_LogMaintenanceConsign::eWaitingForGoingBackToWar: strState = "WaitingForGoingBackToWar"; break;         
        case MOS_LogMaintenanceConsign::eGoingBackToWar          : strState = "GoingBackToWar"; break;         
        case MOS_LogMaintenanceConsign::eFinished                : strState = "Finished"; break;         
        default:
            assert( false );
    }
    pListViewItemState_->setText( 0, MT_FormatString( "Etat : %s\n", strState.c_str() ).c_str() );
}

