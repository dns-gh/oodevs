// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#ifndef __ADN_Missions_WizardPage_h_
#define __ADN_Missions_WizardPage_h_

#include "ADN_WizardPage.h"
#include "ADN_Missions_ABC.h"

// =============================================================================
/** @class  ADN_Missions_WizardPage
    @brief  ADN_Missions_WizardPage
*/
// Created: ABR 2012-07-31
// =============================================================================
class ADN_Missions_WizardPage : public ADN_WizardPage< ADN_Missions_ABC >
{

public:
    //! @name Constructors/Destructor
    //@{
             ADN_Missions_WizardPage( const T_ItemVector& existingItems, const QString& pageTitle, QWidget* pParent = 0, E_MissionType entityType = eNbrMissionTypes )
                 : ADN_WizardPage< ADN_Missions_ABC >( existingItems, pageTitle, pParent )
                 , entityType_( entityType )
             {
                 // NOTHING
             }
    virtual ~ADN_Missions_WizardPage()
    {
        // NOTHING
    }
    //@}

    //! @name Operations
    //@{
    virtual ADN_Missions_ABC* NewT()
    {
        ADN_Missions_Mission* mission = new ADN_Missions_Mission( entityType_ );
        mission->FillContextParameters();
        return mission;
    }
    //@}

private:
    //! @name Member data
    //@{
    E_MissionType entityType_;
    //@}
};

#endif // __ADN_Missions_WizardPage_h_
