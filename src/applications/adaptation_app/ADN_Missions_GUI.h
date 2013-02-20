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
    QWidget* BuildMissions( ADN_Missions_Data::T_Mission_Vector& missions, E_MissionType eMissionType );
    QWidget* BuildFragOrders();
    //@}

private:
    //! @name Member data
    //@{
    ADN_EditLine_ABC* nameFields_[ 4 ];
    QTabWidget* missionTabs_[ 4 ];
    ADN_Missions_Data& data_;
    ADN_HtmlViewer* missionViewers_[ 4 ];
    QSignalMapper* generateMapper_;
    QCheckBox* availableState_;
    //@}

private slots:
    //! @name slots
    //@{
    void OnNotifyElementDeleted( std::string elementName, E_MissionType missionType );
    void OnGenerate( int );
    //@}
};

#endif // __ADN_Missions_GUI_h_
