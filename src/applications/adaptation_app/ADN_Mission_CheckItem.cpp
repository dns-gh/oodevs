// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: AGN 2004-04-28 $
// $Archive: /MVW_v10/Build/SDK/Adn2/src/ADN_Mission_CheckItem.cpp $
// $Author: Ape $
// $Modtime: 18/02/05 15:08 $
// $Revision: 3 $
// $Workfile: ADN_Mission_CheckItem.cpp $
//
// *****************************************************************************

#include "adaptation_app_pch.h"
#include "ADN_Mission_CheckItem.h"
#include "ADN_Tr.h"

#include "ADN_Tools.h"

// -----------------------------------------------------------------------------
// Name: ADN_Mission_CheckItem constructor
/** @param  pParent 
    @param  nMission 
    @param  pMission 
*/
// Created: AGN 2004-04-28
// -----------------------------------------------------------------------------
ADN_Mission_CheckItem::ADN_Mission_CheckItem( QListView* pParent, E_SMission nMission, ADN_Models_Data::MissionInfos* pMission )
: QCheckListItem( pParent, ADN_Tr::ConvertFromSMission( nMission, ENT_Tr_ABC::eToTr ).c_str(), CheckBox )
, eMission_( nMission )
, pMission_( pMission )
{
    if( pMission_ != 0 )
        setState( On );
}

// -----------------------------------------------------------------------------
// Name: ADN_Mission_CheckItem constructor
/** @param  pParent 
    @param  nMission 
    @param  pMission 
*/
// Created: AGN 2004-04-28
// -----------------------------------------------------------------------------
ADN_Mission_CheckItem::ADN_Mission_CheckItem( QCheckListItem* pParent, E_SMission nMission, ADN_Models_Data::MissionInfos* pMission )
: QCheckListItem( pParent, ADN_Tr::ConvertFromSMission( nMission, ENT_Tr_ABC::eToTr ).c_str(), CheckBox )
, eMission_( nMission )
, pMission_( pMission )
{
    if( pMission_ != 0 )
        setState( On );    
}

// -----------------------------------------------------------------------------
// Name: ADN_Mission_CheckItem destructor
// Created: AGN 2004-04-28
// -----------------------------------------------------------------------------
ADN_Mission_CheckItem::~ADN_Mission_CheckItem()
{
    
}
