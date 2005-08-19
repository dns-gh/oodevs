//*****************************************************************************
// 
// $Created: AGN 03-07-21 $
// $Archive: /MVW_v10/Build/SDK/MOS_Light2/src/MOS_LogMaintenanceConsign_ListView_Item.cpp $
// $Author: Age $
// $Modtime: 6/04/05 10:30 $
// $Revision: 2 $
// $Workfile: MOS_LogMaintenanceConsign_ListView_Item.cpp $
// 
//*****************************************************************************

#include "MOS_Light2_pch.h"
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
    , pConsign_                    ( &consign )
    , pListViewItemPion_           ( 0 )
    , pListViewItemLogPionHandling_( 0 )
    , pListViewItemEquipmentType_  ( 0 )
    , pListViewItemBreakdownType_  ( 0 )
    , pListViewItemState_          ( 0 )
{
    // Label
    setText( 0, tr( "Consigne " ) + itostring( pConsign_->GetID() ).c_str() );

    pListViewItemPion_            = new QListViewItem( this                                , tr( "ID Pion demandeur : " ) + consign.GetPion().GetName().c_str() );
    pListViewItemLogPionHandling_ = new QListViewItem( this, pListViewItemPion_            , tr( "Pas de pion log. traitant la consigne" ) );
    pListViewItemEquipmentType_   = new QListViewItem( this, pListViewItemLogPionHandling_ , tr( "Type d'équipement : " ) + MOS_App::GetApp().GetEquipmentName( pConsign_->GetEquipmentTypeID() ).c_str() );
    pListViewItemBreakdownType_   = new QListViewItem( this, pListViewItemEquipmentType_   , tr( "Type de panne : " ) + MOS_App::GetApp().GetBreakDownName( pConsign_->GetBreakdownTypeID() ).c_str() );
    pListViewItemState_           = new QListViewItem( this, pListViewItemBreakdownType_   , "" );

    Update();

    setOpen( true );
}

// -----------------------------------------------------------------------------
// Name: MOS_LogMaintenanceConsign_ListView_Item destructor
// Created: NLD 2004-12-30
// -----------------------------------------------------------------------------
MOS_LogMaintenanceConsign_ListView_Item::~MOS_LogMaintenanceConsign_ListView_Item()
{
    // NOTHING
}

//-----------------------------------------------------------------------------
// Name: MOS_LogMaintenanceConsign_ListView_Item::Update
// Created: AGN 03-07-29*
//-----------------------------------------------------------------------------
void MOS_LogMaintenanceConsign_ListView_Item::Update()
{
    if( pConsign_->GetPionLogHandling() )
        pListViewItemLogPionHandling_->setText( 0, tr( "Pion log. traitant la consigne : " ) + pConsign_->GetPionLogHandling()->GetName().c_str() );
    else
        pListViewItemLogPionHandling_->setText( 0, tr( "Pas de pion log. traitant la consigne" ) );

    MOS_LogMaintenanceConsign::E_State nState = pConsign_->GetState();

    QString strState( tr( "Aucun" ) );
    switch( nState ) 
    {        
        case MOS_LogMaintenanceConsign::eGoingFrom               : strState = tr( "En déplacement vers la chaine" ); break;         
        case MOS_LogMaintenanceConsign::eWaitingForCarrier       : strState = tr( "En attente d'un remorqueur" ); break;         
        case MOS_LogMaintenanceConsign::eCarrierGoingTo          : strState = tr( "Remorqueur en route" ); break;         
        case MOS_LogMaintenanceConsign::eCarrierLoading          : strState = tr( "Remorqueur en cours de chargement" ); break;         
        case MOS_LogMaintenanceConsign::eCarrierGoingFrom        : strState = tr( "Remorqueur en retour" ); break;         
        case MOS_LogMaintenanceConsign::eCarrierUnloading        : strState = tr( "Remorqueur en cours de déchargement" ); break;         
        case MOS_LogMaintenanceConsign::eDiagnosing              : strState = tr( "Diagnostique en cours" ); break;         
        case MOS_LogMaintenanceConsign::eSearchingForUpperLevel  : strState = tr( "En attente de prise en charge par le niveau supérieur" ); break;         
        case MOS_LogMaintenanceConsign::eWaitingForParts         : strState = tr( "En attente de pièces" ); break;         
        case MOS_LogMaintenanceConsign::eWaitingForRepairer      : strState = tr( "En attente d'un réparateur" ); break;         
        case MOS_LogMaintenanceConsign::eRepairing               : strState = tr( "En cours de réparation" ); break;         
        case MOS_LogMaintenanceConsign::eWaitingForGoingBackToWar: strState = tr( "En attente de retour" ); break;         
        case MOS_LogMaintenanceConsign::eGoingBackToWar          : strState = tr( "Retour en cours" ); break;         
        case MOS_LogMaintenanceConsign::eFinished                : strState = tr( "Terminé" ); break;         
        default:
            assert( false );
    }
    pListViewItemState_->setText( 0, tr( "Etat : " ) +  strState );
}

// -----------------------------------------------------------------------------
// Name: MOS_LogMaintenanceConsign_ListView_Item::tr
// Created: AGE 2005-04-01
// -----------------------------------------------------------------------------
QString MOS_LogMaintenanceConsign_ListView_Item::tr( const QString& str ) const
{
    return qApp->tr( str, "MOS_LogMaintenanceConsign_ListView_Item" );
}

// -----------------------------------------------------------------------------
// Name: MOS_LogMaintenanceConsign_ListView_Item::Assign
// Created: AGE 2005-04-06
// -----------------------------------------------------------------------------
void MOS_LogMaintenanceConsign_ListView_Item::Assign( MOS_LogMaintenanceConsign& consign )
{
    pConsign_ = &consign;
    Update();
}
