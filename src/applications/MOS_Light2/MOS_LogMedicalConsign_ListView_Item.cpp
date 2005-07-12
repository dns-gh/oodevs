//*****************************************************************************
// 
// $Created: AGN 03-07-21 $
// $Archive: /MVW_v10/Build/SDK/MOS_Light2/src/MOS_LogMedicalConsign_ListView_Item.cpp $
// $Author: Age $
// $Modtime: 6/04/05 10:38 $
// $Revision: 2 $
// $Workfile: MOS_LogMedicalConsign_ListView_Item.cpp $
// 
//*****************************************************************************

#include "MOS_Light2_Pch.h"
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
    , pConsign_                    ( &consign )
    , pListViewItemPion_           ( 0 )
    , pListViewItemLogPionHandling_( 0 )
    , pListViewItemWound_          ( 0 )
    , pListViewItemMental_         ( 0 )
    , pListViewItemNBC_            ( 0 )
    , pListViewItemState_          ( 0 )
{
    // Label
    setText( 0, tr( "Consigne " ) + itostring( pConsign_->GetID() ).c_str() );

    pListViewItemPion_            = new QListViewItem( this                               , tr( "ID Pion demandeur : " ) + consign.GetPion().GetName().c_str() );
    pListViewItemLogPionHandling_ = new QListViewItem( this, pListViewItemPion_           , tr( "Pas de pion log. traitant la consigne" ) );
    pListViewItemWound_           = new QListViewItem( this, pListViewItemLogPionHandling_, tr( "Blessure : " ) + tr( pConsign_->GetWoundAsString().c_str() ) );
    pListViewItemMental_          = new QListViewItem( this, pListViewItemWound_          , pConsign_->IsMentalDeceased() ? tr( "Reac. mental" ) : tr( "Non reac. mental" ) );
    pListViewItemNBC_             = new QListViewItem( this, pListViewItemMental_         , pConsign_->IsContaminated() ? tr( "Contaminé NBC" ) : tr( "Non contaminé NBC" ) );
    pListViewItemState_           = new QListViewItem( this, pListViewItemNBC_            , "" );

    Update();

    setOpen( true );
}

// -----------------------------------------------------------------------------
// Name: MOS_LogMedicalConsign_ListView_Item destructor
// Created: NLD 2004-12-30
// -----------------------------------------------------------------------------
MOS_LogMedicalConsign_ListView_Item::~MOS_LogMedicalConsign_ListView_Item()
{
    // NOTHING
}

//-----------------------------------------------------------------------------
// Name: MOS_LogMedicalConsign_ListView_Item::Update
// Created: AGN 03-07-29*
//-----------------------------------------------------------------------------
void MOS_LogMedicalConsign_ListView_Item::Update()
{
    if( pConsign_->GetPionLogHandling() )
        pListViewItemLogPionHandling_->setText( 0, tr( "Pion log. traitant la consigne : " ) + pConsign_->GetPionLogHandling()->GetName().c_str() );
    else
        pListViewItemLogPionHandling_->setText( 0, tr( "Pas de pion log. traitant la consigne" ) );

    MOS_LogMedicalConsign::E_State nState = pConsign_->GetState();

    QString strState( tr( "Aucun" ) );
    switch( nState ) 
    {        
        case MOS_LogMedicalConsign::eWaitingForEvacuation               : strState = tr( "En attente d'évacuation" )                    ; break;
        case MOS_LogMedicalConsign::eEvacuationGoingTo                  : strState = tr( "Ambulance en route" )                         ; break;
        case MOS_LogMedicalConsign::eEvacuationLoading                  : strState = tr( "Ambulance en cours de chargement" )           ; break;
        case MOS_LogMedicalConsign::eEvacuationWaitingForFullLoading    : strState = tr( "Ambulance en attente de fin de chargement" )  ; break;
        case MOS_LogMedicalConsign::eEvacuationGoingFrom                : strState = tr( "Ambulance en retour" )                        ; break;
        case MOS_LogMedicalConsign::eEvacuationUnloading                : strState = tr( "Ambulance en cours de déchargement" )         ; break;
        case MOS_LogMedicalConsign::eWaitingForDiagnostic               : strState = tr( "En attente de diagnostique" )                 ; break;
        case MOS_LogMedicalConsign::eDiagnosing                         : strState = tr( "Diagnostique en cours" )                      ; break;
        case MOS_LogMedicalConsign::eSearchingForSortingArea            : strState = tr( "Recherche d'un secteur de tri" )              ; break;
        case MOS_LogMedicalConsign::eWaitingForSorting                  : strState = tr( "En attente de tri" )                          ; break;
        case MOS_LogMedicalConsign::eSorting                            : strState = tr( "Tri en cours" )                               ; break;
        case MOS_LogMedicalConsign::eSearchingForHealingArea            : strState = tr( "Recherche d'un secteur de soin" )             ; break;
        case MOS_LogMedicalConsign::eWaitingForHealing                  : strState = tr( "En attente de soins" )                        ; break;
        case MOS_LogMedicalConsign::eHealing                            : strState = tr( "Soins en cours" )                             ; break;
        case MOS_LogMedicalConsign::eWaitingForCollection               : strState = tr( "En attente de ramassage" )                    ; break;
        case MOS_LogMedicalConsign::eCollectionLoading                  : strState = tr( "Ramassage en cours" )                         ; break;
        case MOS_LogMedicalConsign::eCollectionWaitingForFullLoading    : strState = tr( "En attente de fin de ramassage" )             ; break;
        case MOS_LogMedicalConsign::eCollectionGoingTo                  : strState = tr( "Ramassage en route" )                         ; break;
        case MOS_LogMedicalConsign::eCollectionUnloading                : strState = tr( "Dechargement du ramassage en cours" )         ; break;
        case MOS_LogMedicalConsign::eFinished                           : strState = tr( "Terminé" )                                    ; break;
        default:
            assert( false );
    }
    pListViewItemState_->setText( 0, tr( "Etat : " ) + strState );

    pListViewItemWound_->setText ( 0, tr( "Blessure : " ) + tr( pConsign_->GetWoundAsString().c_str() ) );
    pListViewItemMental_->setText( 0, pConsign_->IsMentalDeceased() ? tr( "Reac. mental" ) : tr( "Non reac. mental" ) );
    pListViewItemNBC_->setText   ( 0, pConsign_->IsContaminated() ? tr( "Contaminé NBC" ) : tr( "Non contaminé NBC" ) );
}

// -----------------------------------------------------------------------------
// Name: MOS_LogMedicalConsign_ListView_Item::tr
// Created: AGE 2005-04-01
// -----------------------------------------------------------------------------
QString MOS_LogMedicalConsign_ListView_Item::tr( const QString& str ) const
{
    return qApp->tr( str, "MOS_LogMedicalConsign_ListView_Item" );
}

// -----------------------------------------------------------------------------
// Name: MOS_LogMedicalConsign_ListView_Item::Assign
// Created: AGE 2005-04-06
// -----------------------------------------------------------------------------
void MOS_LogMedicalConsign_ListView_Item::Assign( MOS_LogMedicalConsign& consign )
{
    pConsign_ = &consign;
    Update();
}
