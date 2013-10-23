// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ADN_Automata_GUI_h_
#define __ADN_Automata_GUI_h_

#include "ADN_GUI_ABC.h"

class ADN_Automata_Data;
class ADN_Automata_ListView;
class ADN_Table;
class ADN_ListView;

// =============================================================================
/** @class  ADN_Automata_GUI
    @brief  ADN_Automata_GUI
*/
// Created: APE 2004-12-06
// =============================================================================
class ADN_Automata_GUI : public ADN_GUI_ABC
{
public:
    enum E_GuiElements
    {
        eName,
        eAgentType,
        eModel,
        eUnit,
        eSubUnit,
        eHasFeedbackTime,
        eFeedbackTime,
        eNbrGuiElements
    };

public:
    //! @name Constructors/Destructor
    //@{
    explicit ADN_Automata_GUI( ADN_Automata_Data& data );
    virtual ~ADN_Automata_GUI();
    //@}

    //! @name Operations
    //@{
    void Build();
    void RegisterTable( ADN_MainWindow& mainWindow );
    //@}

private:
    //! @name Helpers
    //@{
    QWidget* CreateAutomataCompositionsTable();
    QWidget* CreateAutomataLogTable();
    QWidget* CreateAutomataLogTablePerDotation();
    //@}

private:
    //! @name Member data
    //@{
    ADN_Automata_Data& data_;
    //@}
};

#endif // __ADN_Automata_GUI_h_
