// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ADN_Missions_GUI_h_
#define __ADN_Missions_GUI_h_

#include "ADN_GUI_ABC.h"
#include "ADN_Missions_Data.h"
#include "ADN_Models_Data.h"

// =============================================================================
/** @class  ADN_Missions_GUI
    @brief  ADN_Missions_GUI
*/
// Created: APE 2004-12-06
// =============================================================================
class ADN_Missions_GUI : public ADN_GUI_ABC
{

public:
    enum E_GuiElements
    {
        eName,
        eDoctrineDescription,
        eUsageDescription,
        eParameters,
        eParameterValues,
        eFragOrderAvailableWithoutMission,
        eChoiceValues,
        eDiaType,
        eMRTBehavior,
        eCDTBehavior,
        eBehavior,
        eSymbol,
        eNbrGuiElements
    };

    //! @name Constructors/Destructor
    //@{
    explicit ADN_Missions_GUI( ADN_Missions_Data& data );
    virtual ~ADN_Missions_GUI();
    //@}

    //! @name Operations
    //@{
    void Build();
    void Enable( bool enable );
    //@}

private:
    //! @name Helpers
    //@{
    QWidget* BuildMissions( QWidget*& pContent, ADN_Missions_Data::T_Mission_Vector& missions, ADN_Models_Data::ModelInfos::E_ModelEntityType eEntityType );
    QWidget* BuildUnitMissions();
    QWidget* BuildAutomatMissions();
    QWidget* BuildPopulationMissions();
    QWidget* BuildFragOrders();
    //@}

private:
    //! @name Member data
    //@{
    ADN_Missions_Data& data_;
    QWidget* pUnitMissionsWidget_;
    QWidget* pAutomatMissionsWidget_;
    QWidget* pPopulationMissionsWidget_;
    QWidget* pFragOrderWidget_;
    //@}
};


#endif // __ADN_Missions_GUI_h_
