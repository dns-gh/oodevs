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

class ADN_HtmlViewer;

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
        eDotations,
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
    //@}

private:
    //! @name Helpers
    //@{
    QWidget* BuildMissions( E_MissionType eMissionType );
    E_MissionType GetCurrentType() const;
    //@}

private:
    //! @name Member data
    //@{
    ADN_ListView* listViews_[ eNbrMissionType ];
    QTabWidget* missionTabs_[ eNbrMissionType ];
    ADN_Missions_Data& data_;
    ADN_HtmlViewer* missionViewers_[ eNbrMissionType ];
    QGroupBox* helpPanel_[ eNbrMissionType ];
    //@}
    
signals:
    void SendAvailableWithoutMission( bool );

private slots:
    //! @name slots
    //@{
    void OnGenerate( bool changeTab = true );
    void OnHelpNeeded();
    void OnChangeMission();
    //@}
};

#endif // __ADN_Missions_GUI_h_
