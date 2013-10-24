//*****************************************************************************
//
// $Created: JDY 03-07-24 $
// $Archive: /MVW_v10/Build/SDK/Adn2/src/ADN_Units_GUI.h $
// $Author: Ape $
// $Modtime: 22/06/05 17:44 $
// $Revision: 11 $
// $Workfile: ADN_Units_GUI.h $
//
//*****************************************************************************

#ifndef __ADN_Units_GUI_h_
#define __ADN_Units_GUI_h_

#include "ADN_GUI_ABC.h"
#include "ADN_EditLine.h"
#include "ADN_Units_Data.h"
#include "ADN_NavigationInfos.h"
#include "ADN_UnitSymbolWidget.h"

namespace kernel
{
    class SymbolFactory;
    class SymbolRule;
}

class ADN_GuiBuilder;
class ADN_GroupBox;
class ADN_ListView_Units;
class ADN_Nature_GUI;
class ADN_Units_LogThreshold_GUI;
class QComboBox;
class QLabel;
class QWidget;

// =============================================================================
/** @class  ADN_Units_GUI
    @brief  ADN_Units_GUI
*/
// Created: APE 2004-12-06
// =============================================================================
class ADN_Units_GUI : public ADN_GUI_ABC
{
    Q_OBJECT

    friend class ADN_Units;

public:
    //! @name Types
    //@{
    enum E_GuiElements
    {
        eName,
        eTypeId,
        eModel,
        eDecontaminationDelay,
        eStrengthRatioFeedbackTime,
        eProbeWidth,
        eProbeLength,
        eNatureLevel,
        eNatureAtlas,
        eNatureNature,
        eNbOfficer,
        eNbNCOfficer,
        ePostures,
        ePointInfos,
        eComposantes,
        eContenancesTC1,
        eStock,
        eCanProbe,
        eHasStock,
        eHasTC1,
        eHasStrengthRatioFeedbackTime,
        eCanFly,
        eStandardFlyingHeight,
        eTacticalFlyingHeight,
        eCrossingHeight,
        eIsAutonomous,
        eFootprint,
        eHasInstallation,
        eInstallationDelay,
        eUninstallationDelay,
        eFootprintRadius,
        eSpeedModifier,
        eRecon,
        eCombatSupport,
        eCombat,
        eMobilitySupport,
        eCounterMobilitySupport,
        eProtectionSupport,
        eEngineeringRecon,
        eUrbanAreaEfficiency,
        eIsCivilian,
        eMalesPercent,
        eFemalesPercent,
        eChildrenPercent,
        eNatureSymbol,
        eRanges,
        eEquipmentRange,
        eSensorRange,
        eNbcSuit,
        eNbrGuiElements
    };
    //@}

public:
    //! @name Constructors/Destructor
    //@{
    explicit ADN_Units_GUI( ADN_Units_Data& data );
    virtual ~ADN_Units_GUI();
    //@}

    //! @name Operations
    //@{
    void Build();
    void ExportHtml( ADN_HtmlBuilder& mainIndexBuilder, const tools::Path& strPath );
    //@}

private slots:
    //! @name slots
    //@{
    void OnNameChanged();
    void OnTypeChanged();
    void OnNbrOfOfficersChanged();
    void OnNbrOfNCOfficersChanged();
    void OnComponentChanged();
    void OnNatureChanged();
    //@}

private:
    //! @name Helpers
    //@{
    void UpdateValidators();
    void CreateUnitSymbolWidget( ADN_GuiBuilder& builder, const std::string widgetName, QWidget* parent,
                                 ADN_UnitSymbolWidget::T_PixmapExtractor pixmapAccessor,
                                 ADN_UnitSymbolWidget::T_StringExtractor stringAccessor,
                                 QLabel*& pLabel, ADN_UnitSymbolWidget*& pSymbolWidget );
    void SetOnlyApp6SymbolVisible( bool bVisible = true );
    //@}

private:
    //! @name Member data
    //@{
    ADN_Units_Data& data_;
    ADN_EditLine_Int* pOfficersEditLine_;
    ADN_EditLine_Int* pNCOfficersEditLine_;
    QComboBox* pTypeCombo_;
    ADN_GroupBox* pStockGroup_;
    ADN_Units_LogThreshold_GUI* pStockLogThreshold_;
    ADN_GroupBox* pInstallationGroup_;
    ADN_Nature_GUI* pNatureGui_;
    QLabel* labelSymbol_;
    QLabel* labelMoveSymbol_;
    QLabel* labelStaticSymbol_;
    ADN_UnitSymbolWidget* unitSymbolWidget_;
    ADN_UnitSymbolWidget* unitSymbolMoveWidget_;
    ADN_UnitSymbolWidget* unitSymbolStaticWidget_;
    //@}
};

#endif // __ADN_Units_GUI_h_
