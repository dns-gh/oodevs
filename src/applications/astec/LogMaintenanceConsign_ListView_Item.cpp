//*****************************************************************************
// 
// $Created: AGN 03-07-21 $
// $Archive: /MVW_v10/Build/SDK/Light2/src/LogMaintenanceConsign_ListView_Item.cpp $
// $Author: Age $
// $Modtime: 6/04/05 10:30 $
// $Revision: 2 $
// $Workfile: LogMaintenanceConsign_ListView_Item.cpp $
// 
//*****************************************************************************

#include "astec_pch.h"
#include "LogMaintenanceConsign_ListView_Item.h"

#include "App.h"
#include "LogMaintenanceConsign.h"
#include "Agent.h"

// -----------------------------------------------------------------------------
// Name: LogMaintenanceConsign_ListView_Item constructor
// Created: NLD 2004-12-30
// -----------------------------------------------------------------------------
LogMaintenanceConsign_ListView_Item::LogMaintenanceConsign_ListView_Item( QListView* pParent, LogMaintenanceConsign& consign )
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
    pListViewItemEquipmentType_   = new QListViewItem( this, pListViewItemLogPionHandling_ , tr( "Type d'équipement : " ) + App::GetApp().GetEquipmentName( pConsign_->GetEquipmentTypeID() ).c_str() );
    pListViewItemBreakdownType_   = new QListViewItem( this, pListViewItemEquipmentType_   , tr( "Type de panne : " ) + App::GetApp().GetBreakDownName( pConsign_->GetBreakdownTypeID() ).c_str() );
    pListViewItemState_           = new QListViewItem( this, pListViewItemBreakdownType_   , "" );

    Update();

    setOpen( true );
}

// -----------------------------------------------------------------------------
// Name: LogMaintenanceConsign_ListView_Item destructor
// Created: NLD 2004-12-30
// -----------------------------------------------------------------------------
LogMaintenanceConsign_ListView_Item::~LogMaintenanceConsign_ListView_Item()
{
    // NOTHING
}

//-----------------------------------------------------------------------------
// Name: LogMaintenanceConsign_ListView_Item::Update
// Created: AGN 03-07-29*
//-----------------------------------------------------------------------------
void LogMaintenanceConsign_ListView_Item::Update()
{
    if( pConsign_->GetPionLogHandling() )
        pListViewItemLogPionHandling_->setText( 0, tr( "Pion log. traitant la consigne : " ) + pConsign_->GetPionLogHandling()->GetName().c_str() );
    else
        pListViewItemLogPionHandling_->setText( 0, tr( "Pas de pion log. traitant la consigne" ) );

    pListViewItemState_->setText( 0, tr( "Etat : " ) +  QString( pConsign_->GetStateString().c_str() ) );
}

// -----------------------------------------------------------------------------
// Name: LogMaintenanceConsign_ListView_Item::tr
// Created: AGE 2005-04-01
// -----------------------------------------------------------------------------
QString LogMaintenanceConsign_ListView_Item::tr( const QString& str ) const
{
    return qApp->tr( str, "LogMaintenanceConsign_ListView_Item" );
}

// -----------------------------------------------------------------------------
// Name: LogMaintenanceConsign_ListView_Item::Assign
// Created: AGE 2005-04-06
// -----------------------------------------------------------------------------
void LogMaintenanceConsign_ListView_Item::Assign( LogMaintenanceConsign& consign )
{
    pConsign_ = &consign;
    Update();
}
