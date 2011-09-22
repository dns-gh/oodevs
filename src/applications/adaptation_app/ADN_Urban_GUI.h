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

class ADN_Urban_Data;
class ADN_ListView_Urban_Type;
class ADN_ListView_UrbanAccommodation_Type;
class ADN_ListView_UrbanMaterial_Type;
class ADN_ListView_UrbanInfrastructure_Type;
class ADN_Urban_AttritionTable;
class ADN_ListView_Templates;
class ADN_Template_Usages;
class ADN_ListView_RoofShapes;

// =============================================================================
/** @class  ADN_Urban_GUI
    @brief  ADN urban gui
*/
// Created: SLG 2010-03-08
// =============================================================================
class ADN_Urban_GUI : public ADN_GUI_ABC
{
    Q_OBJECT

public:
    //! @name Types
    //@{
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

    enum E_UrbanUsageGuiElements
    {
        eUrbanUsageName,
        eUrbanUsageColor,
        eUrbanUsageAlpha,
        eUrbanUsageHeight,
        eUrbanUsageFloorNumber,
        eUrbanUsageParkingFloors,
        eUrbanUsageOccupation,
        eUrbanUsageTrafficability,
        eUrbanUsageMaterial,
        eUrbanUsageRoofShape,
        eUrbanUsageRole,
        eNbrUrbanUsageGuiElements
    };
    //@}

public:
    //! @name Constructors/Destructor
    //@{
    explicit ADN_Urban_GUI( ADN_Urban_Data& data );
    virtual ~ADN_Urban_GUI();
    //@}

    //! @name Operations
    //@{
    void Build();
    //@}

private:
    //! @name Member Data
    //@{
    ADN_Urban_Data& data_;
    ADN_ListView_UrbanMaterial_Type* pListMaterial_;
    ADN_ListView_Urban_Type* pListFacade_;
    ADN_ListView_RoofShapes* pListRoofShape_;
    ADN_ListView_UrbanAccommodation_Type* pListAccommodation_;
    ADN_ListView_UrbanInfrastructure_Type* pListInfrastructure_;
    ADN_Urban_AttritionTable* pAttritionTable_;
    ADN_ListView_Templates* pListTemplate_;
    ADN_Template_Usages* pUsages_;
    //@}
};

#endif // __ADN_Urban_GUI_h_
