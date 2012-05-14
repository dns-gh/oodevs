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
#include "ADN_EditLine.h"

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
        ePointSize,
        eSymbol,
        eActivableCapacityPresent,
        eAttitudeModifierCapacityPresent,
        eAttitudeModifierCapacity_Attitude,
        eAltitudeModifierCapacityPresent,
        eAttritionCapacity_Dotation,
        eAttritionCapacity_Ph,
        eAttritionCapacity_Surface,
        eAttritionCapacity_UseDotation,
        eAttritionCapacityPresent,
        eAvoidableCapacity_Distance,
        eAvoidableCapacityPresent,
        eBorderCapacityPresent,
        eBridgingCapacity_Type,
        eBridgingCapacityPresent,
        eBuildableCapacity_Dotation,
        eBuildableCapacityPresent,
        eBurnCapacityPresent,
        eBypassableCapacity_Speed,
        eBypassableCapacityPresent,
        eConstructorCapacity_DefaultConsumption,
        eConstructorCapacity_UnitType,
        eConstructorCapacityPresent,
        eContaminationCapacity_MaxToxic,
        eContaminationCapacityPresent,
        eDecontaminationCapacityPresent,
        eDelayCapacityPresent,
        eDetectionCapacity_DetectionTime, // LTO
        eDetectionCapacity_HasDetectionTime, // LTO
        eDetectionCapacity_HasIdentificationTime, // LTO
        eDetectionCapacity_HasRecoTime, // LTO
        eDetectionCapacity_IdentificationTime, // LTO
        eDetectionCapacity_RecoTime, // LTO
        eDetectionCapacityPresent,
        eExtinguishableCapacityPresent,
        eFireForbiddenCapacityPresent,
        eFirePropagationModifierCapacity_Modifiers,
        eFirePropagationModifierCapacityPresent,
        eFloodCapacity_DeadHumans,
        eFloodCapacity_HurtHumans1,
        eFloodCapacity_HurtHumans2,
        eFloodCapacity_HurtHumans3,
        eFloodCapacity_HurtHumansE,
        eFloodCapacityPresent,
        eImprovableCapacity_Dotation,
        eImprovableCapacityPresent,
        eInteractionHeightCapacity_Height,
        eInteractionHeightCapacityPresent,
        eInteractWithSideCapacity_Friend,
        eInteractWithSideCapacity_Enemy,
        eInteractWithSideCapacity_Neutral,
        eInteractWithSideCapacityPresent,
        eInterferenceCapacityPresent,
        eIntoxicationCapacity_MaxToxic,
        eIntoxicationCapacityPresent,
        eLodgingCapacityPresent,
        eLogisticCapacityPresent,
        eMedicalCapacity_EmergencyBedRate,
        eMedicalCapacity_EmergencyDoctorRate,
        eMedicalCapacity_NightRate,
        eMedicalCapacityPresent,
        eMobilityCapacity_DefaultSpeed,
        eMobilityCapacity_MaxAgentSpeed,
        eMobilityCapacity_SpeedModifier,
        eMobilityCapacityPresent,
        eOccupableCapacityPresent,
        ePerceptionCapacity_Blinded,
        ePerceptionCapacityPresent,
        ePopulationCapacity_Density,
        ePopulationCapacityPresent,
        ePropagationCapacity_ModelType,
        ePropagationCapacityPresent,
        eProtectionCapacity_GeniePrepared,
        eProtectionCapacity_MaxSize,
        eProtectionCapacityPresent,
        eResourceNetworkCapacityPresent,
        eScatteringCapacity_HumanByTimeStep,
        eScatteringCapacityPresent,
        eSpawnCapacity_ActionRange,
        eSpawnCapacity_ObjectType,
        eSpawnCapacity_NBC,
        eSpawnCapacityPresent,
        eStockCapacityPresent,
        eStructuralCapacity_Value,
        eStructuralCapacityPresent,
        eSupplyCapacityPresent,
        eSupplyRouteCapacityPresent,
        eTerrainHeuristicCapacity_LocationScore,
        eTerrainHeuristicCapacityPresent,
        eTimeLimitedCapacityPresent,
        eTrafficabilityCapacity_Limited,
        eTrafficabilityCapacity_MaxWeight,
        eTrafficabilityCapacityPresent,
        eUndergroundNetworkCapacityPresent,
        eUniversalCapacityPresent,
        eUrbanDestructionCapacity_Data,
        eUrbanDestructionCapacityPresent,
        eWorkableCapacity_Size,
        eWorkableCapacityPresent,
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
    void OnGeometryChanged( const QString& );
    //@}

private:
    //! @name Member data
    //@{
    ADN_Objects_Data& data_;
    QComboBox* pSpeedImpactCombo_;
    QWidget* pMaxAgentSpeed_;
    ADN_EditLine_Double* pPointDistance_;
    QWidget* pContent_;
    //@}
};


#endif // __ADN_Objects_GUI_h_
