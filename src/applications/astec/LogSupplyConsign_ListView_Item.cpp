//*****************************************************************************
// 
// $Created: AGN 03-07-21 $
// $Archive: /MVW_v10/Build/SDK/Light2/src/LogSupplyConsign_ListView_Item.cpp $
// $Author: Age $
// $Modtime: 6/04/05 10:40 $
// $Revision: 2 $
// $Workfile: LogSupplyConsign_ListView_Item.cpp $
// 
//*****************************************************************************

#include "astec_pch.h"
#include "LogSupplyConsign_ListView_Item.h"

#include "App.h"
#include "LogSupplyConsign.h"
#include "Agent.h"

// -----------------------------------------------------------------------------
// Name: LogSupplyConsign_ListView_Item constructor
// Created: NLD 2004-12-30
// -----------------------------------------------------------------------------
LogSupplyConsign_ListView_Item::LogSupplyConsign_ListView_Item( QListView* pParent, LogSupplyConsign& consign )
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
    pListViewItemLogAutomateProvidingConvoyResources_ = new QListViewItem( this, pListViewItemLogAutomateHandling_                 , tr( "Pas d'automate log fournissant les moyens (camions) du convoi" ) );
    pListViewItemLogPionConvoying_                    = new QListViewItem( this, pListViewItemLogAutomateProvidingConvoyResources_ , tr( "Pas de pion convoi" ) );
    pListViewItemState_                               = new QListViewItem( this, pListViewItemLogPionConvoying_                    , "" );
    pListViewItemDotations_                           = new QListViewItem( this, pListViewItemState_                               , tr( "Dotations demandées/accordées" ) );

    Update();
    setOpen( true );
}

// -----------------------------------------------------------------------------
// Name: LogSupplyConsign_ListView_Item destructor
// Created: NLD 2004-12-30
// -----------------------------------------------------------------------------
LogSupplyConsign_ListView_Item::~LogSupplyConsign_ListView_Item()
{
    // NOTHING
}

//-----------------------------------------------------------------------------
// Name: LogSupplyConsign_ListView_Item::Update
// Created: AGN 03-07-29*
//-----------------------------------------------------------------------------
void LogSupplyConsign_ListView_Item::Update()
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
        pListViewItemLogAutomateProvidingConvoyResources_->setText( 0, tr( "Automate fournissant les moyens (camions) du convoi : " ) + pConsign_->GetAutomateLogProvidingConvoyResources()->GetName().c_str() );
    else
        pListViewItemLogAutomateProvidingConvoyResources_->setText( 0, tr( "Pas d'automate fournissant les moyens (camions) du convoi" ) );

    pListViewItemState_->setText( 0, tr( "Etat : " ) + QString( pConsign_->GetStateString().c_str() ) );

    delete pListViewItemDotations_;
    pListViewItemDotations_ = new QListViewItem( this, pListViewItemState_, tr( "Dotations demandées/accordées/convoyées" ) );
    for( LogSupplyConsign::CIT_DotationMap itDot = pConsign_->GetDotations().begin(); itDot != pConsign_->GetDotations().end(); ++itDot )
    {
        std::stringstream streamItem;
        streamItem << App::GetApp().GetResourceName( itDot->first ) << " : "
                   << itDot->second.nNbrRequested_ << "/"
                   << itDot->second.nNbrReserved_ << "/"
                   << itDot->second.nNbrConvoyed_;
        new QListViewItem( pListViewItemDotations_, streamItem.str().c_str() );
    }
}

// -----------------------------------------------------------------------------
// Name: LogSupplyConsign_ListView_Item::tr
// Created: AGE 2005-04-01
// -----------------------------------------------------------------------------
QString LogSupplyConsign_ListView_Item::tr( const QString& str ) const
{
    return qApp->tr( str, "LogSupplyConsign_ListView_Item" );
}

// -----------------------------------------------------------------------------
// Name: LogSupplyConsign_ListView_Item::Assign
// Created: AGE 2005-04-06
// -----------------------------------------------------------------------------
void LogSupplyConsign_ListView_Item::Assign( LogSupplyConsign& consign )
{
    pConsign_ = &consign;
    Update();
}
