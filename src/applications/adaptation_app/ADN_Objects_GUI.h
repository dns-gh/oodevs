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
        eActionRange,
        eActivableCapacityPresent,
        eAltitudeModifierCapacityPresent,
        eAttitude,
        eAttitudeModifierCapacityPresent,
        eAttritionCapacityPresent,
        eAttritionCapacityUseDotation,
        eAttritionDotation,
        eAvoidableCapacity_Distance,
        eAvoidableCapacityPresent,
        eBlinded,
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
        eDetectionCapacityPresent,
        eDetectionTime, // LTO
        eExtinguishableCapacityPresent,
        eFirePropagationModifier_Modifiers,
        eFirePropagationModifierPresent,
        eFloodCapacity_DeadHumans,
        eFloodCapacity_HurtHumans1,
        eFloodCapacity_HurtHumans2,
        eFloodCapacity_HurtHumans3,
        eFloodCapacity_HurtHumansE,
        eFloodCapacityPresent,
        eHasDetectionTime, // LTO
        eHasIdentificationTime, // LTO
        eHasRecoTime, // LTO
        eHeightInteraction,
        eHeightInteractionPresent,
        eHumanByTimeStep,
        eIdentificationTime, // LTO
        eImprovableCapacity_Dotation,
        eImprovableCapacityPresent,
        eInteractWithEnemyCapacityPresent,
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
        eObjectType,
        eOccupableCapacityPresent,
        ePerceptionCapacityPresent,
        ePopulationCapacityPresent,
        ePopulationFilter_Density,
        ePropagationCapacity_ModelType,
        ePropagationCapacityPresent,
        eProtectionCapacity_GeniePrepared,
        eProtectionCapacity_MaxSize,
        eProtectionCapacityPresent,
        eRecoTime, // LTO
        eResourceNetworkCapacityPresent,
        eScatteringCapacityPresent,
        eSpawnCapacityPresent,
        eStockCapacityPresent,
        eStructuralStateCapacity_Value,
        eStructuralStateCapacityPresent,
        eSupplyCapacityPresent,
        eSupplyRouteCapacityPresent,
        eTerrainHeuristicCapacity_LocationScore,
        eTerrainHeuristicCapacityPresent,
        eTimeLimitedCapacityPresent,
        eUndergroundNetworkCapacityPresent,
        eUniversalCapacityPresent,
        eUrbanDestructionCapacityPresent,
        eUrbanDestructionCapacity_Data,
        eWorkableCapacity_Size,
        eWorkableCapacityPresent,
        eSpawnObjectCapacityPresent,
        eSpawnObjectType,
        eTrafficabilityCapacityPresent,
        eLimitedTrafficability,
        eFireForbiddenCapacityPresent,
        eBorderCapacityPresent,
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
    Q3GroupBox* pGroup_;
    //@}
};


#endif // __ADN_Objects_GUI_h_
