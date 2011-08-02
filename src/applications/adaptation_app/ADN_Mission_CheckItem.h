// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ADN_Mission_CheckItem_h_
#define __ADN_Mission_CheckItem_h_

#include <Qt3Support/q3listview.h>

#include "ADN_Models_Data.h"
#include "ADN_Missions_Data.h"

// =============================================================================
/** @class  ADN_Mission_CheckItem
    @brief  ADN_Mission_CheckItem
*/
// Created: AGN 2004-04-28
// =============================================================================
class ADN_Mission_CheckItem : public Q3CheckListItem
{
public:
    //! @name Constructors/Destructor
    //@{
             ADN_Mission_CheckItem( Q3ListView* pParent, ADN_Missions_Data::Mission& mission, ADN_Models_Data::MissionInfos* pMission = 0 );
             ADN_Mission_CheckItem( Q3CheckListItem* pParent, ADN_Missions_Data::Mission& mission, ADN_Models_Data::MissionInfos* pMission = 0 );
    virtual ~ADN_Mission_CheckItem();
    //@}

    //! @name Accessors
    //@{
    ADN_Missions_Data::Mission& GetMission() const;
    ADN_Models_Data::MissionInfos* GetMissionPtr();
    //@}

private:
    //! @name Copy/Assignment
    //@{
    ADN_Mission_CheckItem( const ADN_Mission_CheckItem& );            //!< Copy constructor
    ADN_Mission_CheckItem& operator=( const ADN_Mission_CheckItem& ); //!< Assignment operator
    //@}

private:
    //! @name Member data
    //@{
    ADN_Missions_Data::Mission&    missionType_;
    ADN_Models_Data::MissionInfos* missionModel_;
    //@}
};


// -----------------------------------------------------------------------------
// Name: ADN_Mission_CheckItem::GetMission
// Created: AGN 2004-04-28
// -----------------------------------------------------------------------------
inline
ADN_Missions_Data::Mission& ADN_Mission_CheckItem::GetMission() const
{
    return missionType_;
}

// -----------------------------------------------------------------------------
// Name: ADN_Mission_CheckItem::GetMissionPtr
// Created: AGN 2004-04-28
// -----------------------------------------------------------------------------
inline
ADN_Models_Data::MissionInfos* ADN_Mission_CheckItem::GetMissionPtr()
{
    return missionModel_;
}


#endif // __ADN_Mission_CheckItem_h_
