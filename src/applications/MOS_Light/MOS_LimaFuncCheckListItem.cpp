//*****************************************************************************
//
// $Created: FBD 03-01-10 $
// $Archive: /MVW_v10/Build/SDK/MOS_Light/src/MOS_LimaFuncCheckListItem.cpp $
// $Author: Nld $
// $Modtime: 19/01/05 10:06 $
// $Revision: 9 $
// $Workfile: MOS_LimaFuncCheckListItem.cpp $
//
//*****************************************************************************

#include "MOS_Light_pch.h"

#include "MOS_LimaFuncCheckListItem.h"

#ifndef MOS_USE_INLINE
#	include "MOS_LimaFuncCheckListItem.inl"
#endif

#include "MOS_LimaListViewItem.h"

//-----------------------------------------------------------------------------
// Name: MOS_LimaFuncCheckListItem constructor
// Created: FBD 03-01-10
//-----------------------------------------------------------------------------
MOS_LimaFuncCheckListItem::MOS_LimaFuncCheckListItem( E_FuncLimaType nFuncType, QListViewItem& item, QCheckListItem::Type nCheckable )
    :   MOS_CheckListItem_ABC( eItemTypeEltFuncLima, item, nCheckable, BuildNameFuncLima( nFuncType ).c_str() )
    ,   nFuncType_( nFuncType )
{
}

//-----------------------------------------------------------------------------
// Name: MOS_LimaFuncCheckListItem destructor
// Created: FBD 03-01-10
//-----------------------------------------------------------------------------
MOS_LimaFuncCheckListItem::~MOS_LimaFuncCheckListItem()
{
    
}


//-----------------------------------------------------------------------------
// Name: std::string 
// Created: FBD 03-01-10
//-----------------------------------------------------------------------------
// static 
std::string MOS_LimaFuncCheckListItem::BuildNameFuncLima( E_FuncLimaType nFuncType )
{
    std::string sName;
    switch( nFuncType )
    {
        case eLimaFuncLD  : sName = "LD - Ligne de débouché"; break;
        case eLimaFuncLCA : sName = "LCA - Ligne de changement d'attitude"; break;
        case eLimaFuncLC  : sName = "LC - Ligne de coordination" ; break;
        case eLimaFuncLI  : sName = "LI - Ligne à interdire"; break;
        case eLimaFuncLO  : sName = "LO - Ligne d'objectif"; break;
        case eLimaFuncLCAR: sName = "LCAR - Ligne de coup d'arrêt"; break;
        case eLimaFuncLR  : sName = "LR - Ligne de recueil"; break;
        case eLimaFuncLDM : sName = "LDM - Ligne de début de mission"; break;
        case eLimaFuncLFM : sName = "LFM - Ligne de fin de mission"; break;
        case eLimaFuncLIA : sName = "LIA - Ligne d'identification et de recueil"; break;
    }
    return sName;
}


//-----------------------------------------------------------------------------
// Name: MOS_LimaFuncCheckListItem::stateChange
// Created: FBD 03-02-28
//-----------------------------------------------------------------------------
void MOS_LimaFuncCheckListItem::stateChange( bool bState )
{
    if ( bState )
        ((MOS_LimaListViewItem*)parent())->FuncStateChanged( *this );
}

