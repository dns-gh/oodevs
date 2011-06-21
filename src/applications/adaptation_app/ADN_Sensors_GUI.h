//*****************************************************************************
//
// $Created: JDY 03-06-30 $
// $Archive: /MVW_v10/Build/SDK/Adn2/src/ADN_Sensors_GUI.h $
// $Author: Nld $
// $Modtime: 3/05/05 14:35 $
// $Revision: 9 $
// $Workfile: ADN_Sensors_GUI.h $
//
//*****************************************************************************

#ifndef __ADN_Sensors_GUI_h_
#define __ADN_Sensors_GUI_h_

#include "ADN_GUI_ABC.h"
#include "ADN_Sensors_Data.h"
#include "ADN_Radars_GUI.h"

class QWidget;
class QTabWidget;
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
