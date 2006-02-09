//*****************************************************************************
// 
// $Created: AGN 03-07-21 $
// $Archive: /MVW_v10/Build/SDK/Light2/src/LogMedicalConsign_ListView_Item.cpp $
// $Author: Age $
// $Modtime: 6/04/05 10:38 $
// $Revision: 2 $
// $Workfile: LogMedicalConsign_ListView_Item.cpp $
// 
//*****************************************************************************

#include "astec_pch.h"
#include "LogMedicalConsign_ListView_Item.h"

#include "App.h"
#include "LogMedicalConsign.h"
#include "Agent.h"

// -----------------------------------------------------------------------------
// Name: LogMedicalConsign_ListView_Item constructor
// Created: NLD 2004-12-30
// -----------------------------------------------------------------------------
LogMedicalConsign_ListView_Item::LogMedicalConsign_ListView_Item( QListView* pParent, LogMedicalConsign& consign )
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
// Name: LogMedicalConsign_ListView_Item destructor
// Created: NLD 2004-12-30
// -----------------------------------------------------------------------------
LogMedicalConsign_ListView_Item::~LogMedicalConsign_ListView_Item()
{
    // NOTHING
}

//-----------------------------------------------------------------------------
// Name: LogMedicalConsign_ListView_Item::Update
// Created: AGN 03-07-29*
//-----------------------------------------------------------------------------
void LogMedicalConsign_ListView_Item::Update()
{
    if( pConsign_->GetPionLogHandling() )
        pListViewItemLogPionHandling_->setText( 0, tr( "Pion log. traitant la consigne : " ) + pConsign_->GetPionLogHandling()->GetName().c_str() );
    else
        pListViewItemLogPionHandling_->setText( 0, tr( "Pas de pion log. traitant la consigne" ) );

    pListViewItemState_->setText( 0, tr( "Etat : " ) + QString( pConsign_->GetStateString().c_str() ) );

    pListViewItemWound_->setText ( 0, tr( "Blessure : " ) + tr( pConsign_->GetWoundAsString().c_str() ) );
    pListViewItemMental_->setText( 0, pConsign_->IsMentalDeceased() ? tr( "Reac. mental" ) : tr( "Non reac. mental" ) );
    pListViewItemNBC_->setText   ( 0, pConsign_->IsContaminated() ? tr( "Contaminé NBC" ) : tr( "Non contaminé NBC" ) );
}

// -----------------------------------------------------------------------------
// Name: LogMedicalConsign_ListView_Item::tr
// Created: AGE 2005-04-01
// -----------------------------------------------------------------------------
QString LogMedicalConsign_ListView_Item::tr( const QString& str ) const
{
    return qApp->tr( str, "LogMedicalConsign_ListView_Item" );
}

// -----------------------------------------------------------------------------
// Name: LogMedicalConsign_ListView_Item::Assign
// Created: AGE 2005-04-06
// -----------------------------------------------------------------------------
void LogMedicalConsign_ListView_Item::Assign( LogMedicalConsign& consign )
{
    pConsign_ = &consign;
    Update();
}
