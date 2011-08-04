// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "adaptation_app_pch.h"
#include "ADN_Mission_CheckItem.h"
#include "ADN_Tr.h"
#include "ADN_Tools.h"

// -----------------------------------------------------------------------------
// Name: ADN_Mission_CheckItem constructor
// Created: AGN 2004-04-28
// -----------------------------------------------------------------------------
ADN_Mission_CheckItem::ADN_Mission_CheckItem( Q3ListView* pParent, ADN_Missions_Data::Mission& mission, ADN_Models_Data::MissionInfos* pMission )
    : Q3CheckListItem( pParent, mission.strName_.GetData().c_str(), CheckBox )
    , missionType_ ( mission )
    , missionModel_( pMission )
{
    if( missionModel_ != 0 )
        setState( On );
}

// -----------------------------------------------------------------------------
// Name: ADN_Mission_CheckItem constructor
// Created: AGN 2004-04-28
// -----------------------------------------------------------------------------
ADN_Mission_CheckItem::ADN_Mission_CheckItem( Q3CheckListItem* pParent, ADN_Missions_Data::Mission& mission, ADN_Models_Data::MissionInfos* pMission )
    : Q3CheckListItem( pParent, mission.strName_.GetData().c_str(), CheckBox )
    , missionType_ ( mission )
    , missionModel_( pMission )
{
    if( missionModel_ != 0 )
        setState( On );
}

// -----------------------------------------------------------------------------
// Name: ADN_Mission_CheckItem destructor
// Created: AGN 2004-04-28
// -----------------------------------------------------------------------------
ADN_Mission_CheckItem::~ADN_Mission_CheckItem()
{
    // NOTHING
}
