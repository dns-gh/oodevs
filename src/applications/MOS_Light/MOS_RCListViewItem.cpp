//*****************************************************************************
//
// $Created: NLD 2002-07-16 $
// $Archive: /MVW_v10/Build/SDK/MOS_Light/src/MOS_RCListViewItem.cpp $
// $Author: Nld $
// $Modtime: 4/05/05 16:11 $
// $Revision: 31 $
// $Workfile: MOS_RCListViewItem.cpp $
//
//*****************************************************************************

#include "MOS_Light_pch.h"
#include "MOS_RCListViewItem.h"

#ifndef MOS_USE_INLINE
#   include "MOS_RCListViewItem.inl"
#endif

#include "MOS_RC.h"
#include "MOS_App.h"
#include "MOS_World.h"
#include "MOS_RCListView.h"
#include "MOS_Agent.h"
#include "MOS_RCSubItem.h"



using namespace DIN;

//-----------------------------------------------------------------------------
// Name: MOS_RCListViewItem constructor
// Created: NLD 2002-07-17
//-----------------------------------------------------------------------------
MOS_RCListViewItem::MOS_RCListViewItem( MOS_RC& RC, QListView& listView, QListViewItem& itemAfter, const std::string& strField, const std::string& strValue )
    : MOS_ListViewItem_ABC( eItemTypeEltRC, listView, itemAfter, strField, strValue )
    , RC_          ( RC )
{
    Initialize();
}

//-----------------------------------------------------------------------------
// Name: MOS_RCListViewItem constructor
// Created: NLD 2002-07-17
//-----------------------------------------------------------------------------
MOS_RCListViewItem::MOS_RCListViewItem( MOS_RC& RC, QListViewItem& item, const std::string& strField, const std::string& strValue )
    : MOS_ListViewItem_ABC( eItemTypeEltRC, item, strField, strValue )
    , RC_          ( RC )
{
    Initialize();
}

//-----------------------------------------------------------------------------
// Name: MOS_RCListViewItem constructor
// Created: NLD 2002-07-17
//-----------------------------------------------------------------------------
MOS_RCListViewItem::MOS_RCListViewItem( MOS_RC& RC, QListView& listView, const std::string& strField, const std::string& strValue )
    : MOS_ListViewItem_ABC( eItemTypeEltRC, listView, strField, strValue )
    , RC_          ( RC )
{
    Initialize();
}


//-----------------------------------------------------------------------------
// Name: MOS_RCListViewItem::Initialize
// Created: AGN 03-06-03
//-----------------------------------------------------------------------------
void MOS_RCListViewItem::Initialize()
{
    CreateRect();
    setRenameEnabled( 0, false );

    // Register to the RC_ and let the RC initialize us
    RC_.SetRCItem( this );

    setText( 0, MT_FormatString( "%02d:%02d:%02d", ( RC_.GetTime() / 3600 ) % 24, ( RC_.GetTime() / 60 ) % 60 , RC_.GetTime() % 60  ).c_str() );        
    setText( 1, RC_.GetTitle().c_str() );

    const T_StringVector& textFields = RC_.GetTextFields();
    for( CIT_StringVector itText = textFields.begin(); itText != textFields.end(); ++itText )
        AddText( *itText );

    const MOS_RC::T_OrderConduiteVector& orderConduites = RC_.GetOrderConduites();
    for( MOS_RC::CIT_OrderConduiteVector itOrderConduite = orderConduites.begin(); itOrderConduite != orderConduites.end(); ++itOrderConduite )
        AddOrderConduite( *itOrderConduite );
}


//-----------------------------------------------------------------------------
// Name: MOS_RCListViewItem::CreateRect
// Created: FBD 03-01-27
//-----------------------------------------------------------------------------
void MOS_RCListViewItem::CreateRect()
{
    MT_Vector2D point = RC_.GetPos();
    MT_Float rSizeButton = 500;
    vPos1_ = MT_Vector2D( point.rX_ - rSizeButton, point.rY_ + rSizeButton);
    vPos2_ = MT_Vector2D( point.rX_ + rSizeButton, point.rY_ + rSizeButton);
    vPos3_ = MT_Vector2D( point.rX_, point.rY_ - rSizeButton);
}


//-----------------------------------------------------------------------------
// Name: MOS_RCListViewItem destructor
// Created: NLD 2002-07-16
//-----------------------------------------------------------------------------
MOS_RCListViewItem::~MOS_RCListViewItem()
{
    RC_.SetRCItem( 0 );
}

 
//-----------------------------------------------------------------------------
// Name: MOS_RCListViewItem::Draw
// Created: FBD 02-12-17
//-----------------------------------------------------------------------------
void MOS_RCListViewItem::Draw( MOS_RCListViewItem* pSelectedRC ) const
{
    if ( pSelectedRC == this )
    {
        GFX_Color colorTriangle = GFX_Color( 000. ,000. ,255., 0.4 );
        GFX_Tools::CreateGLTriangleShadow( vPos1_, vPos2_, vPos3_, 1., 6., colorTriangle  );
    }
}


//-----------------------------------------------------------------------------
// Name: MOS_RCListViewItem::IsInside
// Created: FBD 02-12-17
//-----------------------------------------------------------------------------
bool MOS_RCListViewItem::IsInside( const MT_Vector2D& vPos ) const
{
    MT_Triangle triangle( vPos1_, vPos2_, vPos3_ );
    
    if ( triangle.IsInside( vPos ) )
        return true;
    return 0;
}


//-----------------------------------------------------------------------------
// Name: MOS_RCListViewItem::AddOrderConduite
// Created: FBD 03-01-28
//-----------------------------------------------------------------------------
MOS_RCSubItem& MOS_RCListViewItem::AddOrderConduite( E_FragOrder nOrdreConduite )
{
    MOS_RCSubItem& item = CreateItem( eItemTypeEltOrderConduite, nOrdreConduite );
    item.SetName( BuildNameOrderConduite( nOrdreConduite ) );
    return item;
}


//-----------------------------------------------------------------------------
// Name: MOS_RCListViewItem::AddText
// Created: NLD 2003-01-29
//-----------------------------------------------------------------------------
MOS_RCSubItem& MOS_RCListViewItem::AddText( const std::string& strText )
{
    MOS_RCSubItem& item = CreateItem( eItemTypeEltText );
    item.SetName( strText );
    return item;
}


//-----------------------------------------------------------------------------
// Name: std::string 
// Created: FBD 03-01-28
//-----------------------------------------------------------------------------
const std::string MOS_RCListViewItem::BuildNameOrderConduite( E_FragOrder nOrderConduite ) const
{
    std::string sName;

    switch( nOrderConduite )
    {
        case eOrdreConduite_ChangerReglesEngagement:                        sName = "Conduite ChangeReglesEngagement"; break;
        case eOrdreConduite_Pion_StopperEmission:                           sName = "Conduite StopperEmission"; break;
        case eOrdreConduite_Pion_EnvoyerVehicule:                           sName = "Conduite EnvoyerVehicule"; break;
        case eOrdreConduite_AttendreSePoster:                               sName = "Conduite AttendreSePoster"; break;
        case eOrdreConduite_Poursuivre:                                     sName = "Conduite Poursuivre"; break;
        case eOrdreConduite_Interrompre:                                    sName = "Conduite Interrompre"; break;
        case eOrdreConduite_Deboucher :                                     sName = "Conduite Deboucher"; break;
        case eOrdreConduite_Decrocher :                                     sName = "Conduite Decrocher"; break;
        case eOrdreConduite_AcquerirObjectif:                               sName = "Conduite AcquerirObjectif"; break;
        case eOrdreConduite_Brouiller:                                      sName = "Conduite Brouiller"; break;
        case eOrdreConduite_ChangerPositionDebarquement:                    sName = "Conduite ChangerPositionDebarquement"; break;

        case eOrdreConduite_MettreTenueNBC:                                 sName = "Conduite MettreTenueNBC"; break;
        case eOrdreConduite_EnleverTenueNBC:                                sName = "Conduite EnleverTenueNBC"; break;        
        case eOrdreConduite_PasserEnSilenceRadio:                           sName = "Conduite PasserEnSilenceRadio"; break;
        case eOrdreConduite_ArreterSilenceRadio:                            sName = "Conduite ArreterSilenceRadio"; break;
        case eOrdreConduite_PasserEnSilenceRadar:                           sName = "Conduite PasserEnSilenceRadar"; break;
        case eOrdreConduite_ArreterSilenceRadar:                            sName = "Conduite ArreterSilenceRadar"; break;
        case eOrdreConduite_RecupererTransporteurs:                         sName = "Conduite RecupererTransporteurs"; break;
        case eOrdreConduite_Pion_Contourner:                                sName = "Conduite Pion Contourner"; break;
        case eOrdreConduite_Pion_ActiverObstacle:                           sName = "Conduite Pion ActiverObstacle"; break;
        case eOrdreConduite_Pion_ChangerDePosition:                         sName = "Conduite Pion ChangerDePosition"; break;
        case eOrdreConduite_Pion_ExecuterTir:                               sName = "Conduite Pion ExecuterTir"; break;
        case eOrdreConduite_Pion_AppliquerFeux:                             sName = "Conduite Pion AppliquerFeux"; break;
        case eOrdreConduite_Pion_Emettre:                                   sName = "Conduite Pion Emettre"; break;

        case eOrdreConduite_Pion_RenforcerEnVSRAM:                          sName = "Conduite Pion RenforcerEnVSRAM"; break;
        case eOrdreConduite_Pion_TransfererVSRAM:                           sName = "Conduite Pion TransfererVSRAM"; break;
        case eOrdreConduite_Pion_ReprendreAuxOrdresVSRAM:                   sName = "Conduite Pion ReprenreAuxOrdresVSRAM"; break;
        case eOrdreConduite_ModifierTempsBordeeMaintenance:                       sName = "Conduite ModifierTempsBordeeMaintenance"; break;
        case eOrdreConduite_ModifierTempsBordeeSante:                       sName = "Conduite ModifierTempsBordeeSante"; break;
        case eOrdreConduite_ModifierPrioritesReparations:               sName = "Conduite ModifierPrioritesReparations"; break;
        case eOrdreConduite_ModifierPrioritesBlesses:                  sName = "Conduite ModifierPrioritesBlesses"; break;

        case eOrdreConduite_Automate_ReconnaitreZoneImplantation   :        sName = "Conduite Automate ReconnaitreZoneImplantation"; break;
        case eOrdreConduite_Automate_RealiserVariantement:                  sName = "Conduite Automate RealiserVariantement"; break;
        case eOrdreConduite_Automate_DesequiperSiteFranchissement:          sName = "Conduite Automate DesequiperSiteFranchissement"; break;
        case eOrdreConduite_Automate_ReagirFaceAEni:                        sName = "Conduite Automate ReagirFaceAEni"; break;
        case eOrdreConduite_Automate_AffecterPionAObstacle:                 sName = "Conduite Automate AffecterPionAObstacle"; break;


        default: 
            assert( false );
    }
    return sName;
}

//-----------------------------------------------------------------------------
// Name: MOS_RCListViewItem::AddOrderConduite
// Created: FBD 03-01-28
//-----------------------------------------------------------------------------
MOS_RCSubItem& MOS_RCListViewItem::CreateItem( E_ItemType nType, int nValue )
{
    if ( itemVector_.size() )
    {
        MOS_RCSubItem& item = *new MOS_RCSubItem( nType, *this, **itemVector_.rbegin(), "" );
        item.SetValue( nValue );
        RegisterItem( item );
        return item;
    }
    else
    {
        MOS_RCSubItem& item = *new MOS_RCSubItem( nType, *this, "" );
        item.SetValue( nValue );
        RegisterItem( item );
        return item;
    }
}


//-----------------------------------------------------------------------------
// Name: MOS_RCListViewItem::CreateItem
// Created: NLD 2003-01-29
//-----------------------------------------------------------------------------
MOS_RCSubItem& MOS_RCListViewItem::CreateItem( E_ItemType nType, const std::string& strText )
{
    if ( itemVector_.size() )
    {
        MOS_RCSubItem& item = *new MOS_RCSubItem( nType, *this, **itemVector_.rbegin(), strText.c_str() );
        RegisterItem( item );
        return item;
    }
    else
    {
        MOS_RCSubItem& item = *new MOS_RCSubItem( nType, *this, strText.c_str() );
        RegisterItem( item );
        return item;
    }      
}

//-----------------------------------------------------------------------------
// Name: MOS_RCListViewItem::AddOrderConduite
// Created: FBD 03-01-28
//-----------------------------------------------------------------------------
void MOS_RCListViewItem::DeleteItem( MOS_RCSubItem& item )
{
    UnregisterItem( item );
    delete &item;
}


//-----------------------------------------------------------------------------
// Name: MOS_RCListViewItem::DeleteAllItems
// Created: NLD 2003-01-29
//-----------------------------------------------------------------------------
void MOS_RCListViewItem::DeleteAllItems()
{
    while( itemVector_.size() )
    {
        DeleteItem( **itemVector_.rbegin() );
    }
}
//-----------------------------------------------------------------------------
// Name: MOS_RCListViewItem::RegisterItem
// Created: FBD 03-01-28
//-----------------------------------------------------------------------------
void MOS_RCListViewItem::RegisterItem( MOS_RCSubItem& item )
{
    IT_RCItemPtrVector itItem = std::find( itemVector_.begin(), itemVector_.end(), &item );
    assert ( itItem == itemVector_.end() );
    itemVector_.push_back( &item );
}


//-----------------------------------------------------------------------------
// Name: MOS_RCListViewItem::UnregisterItem
// Created: FBD 03-01-28
//-----------------------------------------------------------------------------
void MOS_RCListViewItem::UnregisterItem( MOS_RCSubItem& item )
{
    IT_RCItemPtrVector itItem = std::find( itemVector_.begin(), itemVector_.end(), &item );
    assert ( itItem != itemVector_.end() );
    itemVector_.erase( itItem );
}


