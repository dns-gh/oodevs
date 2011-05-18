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
        eAttritionDotation,
        eAttritionCapacityUseDotation,
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
        eExtinguishableCapacityPresent,
        eMedicalCapacityPresent,
        eMedicalCapacity_NightRate,
        eMedicalCapacity_EmergencyDoctorRate,
        eMedicalCapacity_EmergencyBedRate,
        eImprovableCapacityPresent,
        eImprovableCapacity_Dotation,
        eInterferenceCapacityPresent,
        eInteractWithEnemyCapacityPresent,
        eHeightInteractionPresent,
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
        eDetectionCapacityPresent,
        eHasDetectionTime, // LTO
        eDetectionTime, // LTO
        eHasRecoTime, // LTO
        eRecoTime, // LTO
        eHasIdentificationTime, // LTO
        eIdentificationTime, // LTO
        eSpawnCapacityPresent,
        eActionRange,
        eObjectType,
        eAttitudeModifierCapacityPresent,
        eAttitude,
        ePerceptionCapacityPresent,
        eBlinded,
        eScatteringCapacityPresent,
        eHumanByTimeStep,
        eDelayCapacityPresent,
        eStructuralStateCapacityPresent,
        eStructuralStateCapacity_Value,
        eFirePropagationModifierPresent,
        eFirePropagationModifier_Modifiers,
        eBurnCapacityPresent,
        eHeightInteraction,
        eFloodCapacityPresent,
        eFloodCapacity_HurtHumans1,
        eFloodCapacity_HurtHumans2,
        eFloodCapacity_HurtHumans3,
        eFloodCapacity_HurtHumansE,
        eFloodCapacity_DeadHumans,
        eUniversalCapacityPresent,
        eStockCapacityPresent,
        ePopulationCapacityPresent,
        ePopulationFilter_Density,
        eResourceNetworkCapacityPresent,
        eLodgingCapacityPresent,
        eAltitudeModifierCapacityPresent,
        eNbrGuiElements
    };

public:
    //! @name Constructors/Destructor
    //@{
    explicit ADN_Objects_GUI( ADN_Objects_Data& data );
    virtual ~ADN_Objects_GUI();
    //@}

    //! @name Operations
    //@{
    void Build();
    void Enable( bool enable );
    void ExportHtml( ADN_HtmlBuilder& mainIndexBuilder, const QString& strPath );
    //@}

    private slots:
        //! @name slots
        //@{
        void OnSpeedImpactComboChanged();
        //@}

private:
    //! @name Member data
    //@{
    ADN_Objects_Data& data_;
    QComboBox* pSpeedImpactCombo_;
    QWidget* pMaxAgentSpeed_;
    QGroupBox* pGroup_;
    //@}
};


#endif // __ADN_Objects_GUI_h_
