// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ADN_Urban_GUI_h_
#define __ADN_Urban_GUI_h_

#include "ADN_GUI_ABC.h"
#include <QtCore/qobject.h>

class ADN_Urban_Data;
class ADN_ListView_Urban_Type;
class ADN_ListView_UrbanAccommodation_Type;
class ADN_ListView_UrbanMaterial_Type;
class ADN_ListView_UrbanInfrastructure_Type;
class ADN_Urban_AttritionTable;

// =============================================================================
/** @class  ADN_Urban_GUI
    @brief  ADN urban gui
*/
// Created: SLG 2010-03-08
// =============================================================================
class ADN_Urban_GUI
    : public ADN_GUI_ABC
{
    Q_OBJECT

public:

    enum E_UrbanGuiElements
    {
        eUrbanName,
        eNbrUrbanGuiElements
    };

    enum E_UrbanMaterialGuiElements
    {
        eUrbanMaterialName,
        eUrbanMaterialAttrition,
        eNbrUrbanMaterialGuiElements
    };

    enum E_UrbanAccommodationGuiElements
    {
        eUrbanAccommodationName,
        eUrbanAccommodationNominalCapacity,
        eUrbanAccommodationMaxCapacity,
        eNbrUrbanAccommodationGuiElements
    };

    enum E_UrbanInfrastructureGuiElements
    {
        eUrbanInfrastructureName,
        eUrbanInfrastructureSymbol,
        eMedicalCapacityPresent,
        eMedicalCapacity_NightRate,
        eMedicalCapacity_EmergencyDoctorRate,
        eMedicalCapacity_EmergencyBedRate,
        eNbrUrbanInfrastructureGuiElements
    };

public:
    explicit ADN_Urban_GUI( ADN_Urban_Data& data );
    virtual ~ADN_Urban_GUI();

    void Build();

private:
    ADN_Urban_Data& data_;
    ADN_ListView_UrbanMaterial_Type* pListMaterial_;
    ADN_ListView_Urban_Type* pListFacade_;
    ADN_ListView_Urban_Type* pListRoofShape_;
    ADN_ListView_UrbanAccommodation_Type* pListAccommodation_;
    ADN_ListView_UrbanInfrastructure_Type* pListInfrastructure_;
    ADN_Urban_AttritionTable* pAttritionTable_;
};

#endif // __ADN_Urban_GUI_h_
