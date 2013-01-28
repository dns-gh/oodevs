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
#include "ADN_EditLine_ABC.h"

enum E_EntityType;
class ADN_HtmlViewer;

namespace gui
{
    class HtmlEditor;
}

// =============================================================================
/** @class  ADN_Missions_GUI
    @brief  ADN_Missions_GUI
*/
// Created: APE 2004-12-06
// =============================================================================
class ADN_Missions_GUI : public ADN_Tabbed_GUI_ABC
{
    Q_OBJECT

public:
    enum E_GuiElements
    {
        eName,
        //mission sheets
        eDescriptionSheetPath,
        eDescriptionContext,
        eDescriptionParameters,
        eDescriptionParametersText,
        eDescriptionBehavior,
        eDescriptionSpecificCases,
        eDescriptionComments,
        eDescriptionMissionEnd,
        eDescriptionAttachments,
        //--------------
        eParameters,
        eParameterValues,
        eFragOrderAvailableWithoutMission,
        eChoiceValues,
        eGenObjects,
        eKnowledgeObjects,
        eMinValue,
        eMaxValue,
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
    QWidget* BuildMissions( QTabWidget*& pContent, ADN_Missions_Data::T_Mission_Vector& missions, E_EntityType eEntityType );
    QWidget* BuildUnitMissions();
    QWidget* BuildAutomatMissions();
    QWidget* BuildPopulationMissions();
    QWidget* BuildFragOrders();
    //@}

private:
    //! @name Member data
    //@{
    ADN_EditLine_ABC* nameField_;
    ADN_Missions_Data& data_;
    QTabWidget* pUnitMissionsWidget_;
    QTabWidget* pAutomatMissionsWidget_;
    QTabWidget* pPopulationMissionsWidget_;
    QTabWidget* fragOrderDescriptionTab_;
    ADN_HtmlViewer* missionViewer_;
    ADN_HtmlViewer* fragViewer_;
    //@}

private slots:
    //! @name slots
    //@{
    void OnNotifyElementDeleted( std::string elementName, E_EntityType elementType );
    void OnGenerateUnitMissionSheet();
    void OnGenerateAutomataMissionSheet();
    void OnGeneratePopulationMissionSheet();
    void OnGenerateFragOrdersMissionSheet();
    //@}
};

#endif // __ADN_Missions_GUI_h_
