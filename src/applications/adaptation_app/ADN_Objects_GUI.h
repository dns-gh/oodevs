//*****************************************************************************
//
// $Created: JDY 03-06-25 $
// $Archive: /MVW_v10/Build/SDK/ADN2/src/ADN_Objects_GUI.h $
// $Author: Nld $
// $Modtime: 29/04/05 16:37 $
// $Revision: 9 $
// $Workfile: ADN_Objects_GUI.h $
//
//*****************************************************************************

#ifndef __ADN_Objects_GUI_h_
#define __ADN_Objects_GUI_h_

#include "ADN_GUI_ABC.h"

class QWidget;
class ADN_Objects;
class ADN_Objects_Data;


// =============================================================================
/** @class  ADN_Objects_GUI
    @brief  ADN_Objects_GUI
*/
// Created: APE 2004-12-06
// =============================================================================
class ADN_Objects_GUI 
: public ADN_GUI_ABC
{
    Q_OBJECT

public:
    enum E_GuiElements
    {
        eName = 0,
        eGeometry,
        eSymbol,

        eActivableCapacityPresent,

		eAttritionCapacityPresent,

        eAvoidableCapacityPresent,
        eAvoidableCapacity_Distance,

        eBridgingCapacityPresent,

        eBuildableCapacityPresent,
        eBuildableCapacity_Dotation,

        eBypassableCapacityPresent,
        eBypassableCapacity_Speed,

        eConstructorCapacityPresent,
        eConstructorCapacity_DefaultConsumption,
        eConstructorCapacity_UnitType,

		eContaminationCapacityPresent,
        eContaminationCapacity_MaxToxic,

		eDecontaminationCapacityPresent,

		eDetectionCapacityPresent,

		eExtinguishableCapacityPresent,

		eHealableCapacityPresent,    

        eImprovableCapacityPresent,        
        eImprovableCapacity_Dotation,

		eInterferenceCapacityPresent,

        eIntoxicationCapacityPresent,
        eIntoxicationCapacity_MaxToxic,

        eLogisticCapacityPresent,

        eMobilityCapacityPresent,
        eMobilityCapacity_DefaultSpeed,
        eMobilityCapacity_SpeedModifier,
        eMobilityCapacity_MaxAgentSpeed,

		eOccupableCapacityPresent,

		ePropagationCapacityPresent,
        ePropagationCapacity_ModelType,

		eProtectionCapacityPresent,
        eProtectionCapacity_MaxSize,
        eProtectionCapacity_GeniePrepared,
 
        eSupplyCapacityPresent,

        eSupplyRouteCapacityPresent,

		eTerrainHeuristicCapacityPresent,
        eTerrainHeuristicCapacity_LocationScore,

        eTimeLimitedCapacityPresent,

		eWorkableCapacityPresent,
        eWorkableCapacity_Size,

        eNbrGuiElements
    };

public:
    //! @name Constructors/Destructor
    //@{
    ADN_Objects_GUI( ADN_Objects_Data& data );
    ~ADN_Objects_GUI();
    //@}

    //! @name Operations
    //@{
    void Build();
    //@}

private slots:
    void OnSpeedImpactComboChanged();

private:
    //! @name Member data
    //@{
    ADN_Objects_Data& data_;

    QComboBox* pSpeedImpactCombo_;
    QWidget* pMaxAgentSpeed_;
    //@}
};


#endif // __ADN_Objects_GUI_h_
