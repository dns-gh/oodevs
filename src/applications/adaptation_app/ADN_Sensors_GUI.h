// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2003 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ADN_Sensors_GUI_h_
#define __ADN_Sensors_GUI_h_

#include "ADN_GUI_ABC.h"
#include "ADN_Sensors_Data.h"
#include "ADN_Radars_GUI.h"

class ADN_Table;

// =============================================================================
/** @class  ADN_Sensors_GUI
    @brief  ADN_Sensors_GUI
*/
// Created: APE 2004-12-07
// =============================================================================
class ADN_Sensors_GUI : public ADN_GUI_ABC
{
    Q_OBJECT

public:
    enum E_GuiElemements
    {
        eName,
        eAngle,
        eCanScan,
        eDistFirerReco,
        eDistProximity,
        eDistDetection,
        eDistReco,
        eDistIdent,
        eModifSizes,
        eModifWeather,
        eModifIllumination,
        eModifEnvironement,
        eModifUrbanBlockMaterial,
        eModifStances,
        eModifTargetStances,
        eTargets,
        eCanDetectAgents,
        eCanDetectObjects,
        ePopulationDensity,
        ePopulationModifier,
        eDetectionDelay,
        eLimitedSensorsList, // LTO
        eLimitedToSensors, // LTO
        ePreviewModifSizes,
        ePreviewModifWeather,
        ePreviewModifIllumination,
        ePreviewModifEnvironement,
        ePreviewModifUrbanBlockMaterial,
        ePreviewModifStances,
        ePreviewModifTargetStances,
        eNbrGuiElements
    };

    enum E_ObjGuiElements
    {
        eObjDistDetect,
        eObjModifStances,
        eObjPopulationDensity,
        eObjPopulationModifier,
        eNbrObjGuiElements
    };

public:
    //! @name Constructors/Destructor
    //@{
    explicit ADN_Sensors_GUI( ADN_Sensors_Data& data );
    virtual ~ADN_Sensors_GUI();
    //@}

    //! @name Operations
    //@{
    void Build();
    void RegisterTable( ADN_MainWindow& mainWindow );

    ADN_Table* CreateAgentDetectionTable();
    ADN_Table* CreateObjectDetectionTable();
    //@}

private:
    //! @name Helpers
    //@{
    void BuildSensorListGui( QTabWidget* pParent );
    void BuildSpecificParamsGui( QTabWidget* pParent );
    void AddHeaders( ADN_Table* pTable, int& nCol, const char* szName, const ADN_Categories_Data::T_SizeInfos_Vector& materials );
    //@}

private:
    //! @name Member data
    //@{
    ADN_Sensors_Data& data_;
    ADN_Radars_GUI& radarGui_;
    //@}
};

#endif // __ADN_Sensors_GUI_h_
