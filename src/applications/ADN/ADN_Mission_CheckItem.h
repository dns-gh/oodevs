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
// $Archive: /MVW_v10/Build/SDK/Adn2/src/ADN_Mission_CheckItem.h $
// $Author: Ape $
// $Modtime: 18/02/05 11:58 $
// $Revision: 3 $
// $Workfile: ADN_Mission_CheckItem.h $
//
// *****************************************************************************

#ifndef __ADN_Mission_CheckItem_h_
#define __ADN_Mission_CheckItem_h_

#include <qlistview.h>

#include "ADN_Models_Data.h"

// =============================================================================
/** @class  ADN_Mission_CheckItem
    @brief  ADN_Mission_CheckItem
    @par    Using example
    @code
    ADN_Mission_CheckItem;
    @endcode
*/
// Created: AGN 2004-04-28
// =============================================================================
class ADN_Mission_CheckItem
: public QCheckListItem
{

public:
    //! @name Constructors/Destructor
    //@{
             ADN_Mission_CheckItem( QListView* pParent, E_SMission nMission, ADN_Models_Data::MissionInfos* pMission = 0 );
             ADN_Mission_CheckItem( QCheckListItem* pParent, E_SMission nMission, ADN_Models_Data::MissionInfos* pMission = 0 );
    virtual ~ADN_Mission_CheckItem();
    //@}

    //! @name Accessors
    //@{
    E_SMission GetMission() const;
    ADN_Models_Data::MissionInfos* GetMissionPtr();
    //@}

    //! @name Modifiers
    //@{
    //@}

    //! @name Operators
    //@{
    //@}

private:
    //! @name Copy/Assignement
    //@{
    ADN_Mission_CheckItem( const ADN_Mission_CheckItem& );            //!< Copy constructor
    ADN_Mission_CheckItem& operator=( const ADN_Mission_CheckItem& ); //!< Assignement operator
    //@}

    //! @name Helpers
    //@{
    //@}

private:
    //! @name Member data
    //@{
    E_SMission eMission_;
    ADN_Models_Data::MissionInfos* pMission_;
    //@}
};


// -----------------------------------------------------------------------------
// Name: ADN_Mission_CheckItem::GetMission
// Created: AGN 2004-04-28
// -----------------------------------------------------------------------------
inline
E_SMission ADN_Mission_CheckItem::GetMission() const
{
    return eMission_;
}

// -----------------------------------------------------------------------------
// Name: ADN_Mission_CheckItem::GetMissionPtr
// Created: AGN 2004-04-28
// -----------------------------------------------------------------------------
inline
ADN_Models_Data::MissionInfos* ADN_Mission_CheckItem::GetMissionPtr()
{
    return pMission_;
}


#endif // __ADN_Mission_CheckItem_h_
