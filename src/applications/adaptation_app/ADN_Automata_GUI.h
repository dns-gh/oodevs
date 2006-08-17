// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: APE 2004-12-06 $
// $Archive: /MVW_v10/Build/SDK/Adn2/src/ADN_Automata_GUI.h $
// $Author: Ape $
// $Modtime: 4/04/05 11:32 $
// $Revision: 7 $
// $Workfile: ADN_Automata_GUI.h $
//
// *****************************************************************************

#ifndef __ADN_Automata_GUI_h_
#define __ADN_Automata_GUI_h_

#include "ADN_GUI_ABC.h"

class ADN_Automata_Data;
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
    MT_COPYNOTALLOWED( ADN_Automata_GUI )

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
     ADN_Automata_GUI( ADN_Automata_Data& data );
    ~ADN_Automata_GUI();
    //@}

    //! @name Operations
    //@{
    void Build();
    void RegisterTable( ADN_MainWindow& mainWindow );

    ADN_Table*    CreateAutomataCompositionsTable  ();
    ADN_ListView* CreateAutomataLogTable           ();
    ADN_ListView* CreateAutomataLogTablePerDotation();
    //@}

private:
    //! @name Member data
    //@{
    ADN_Automata_Data& data_;
    //@}
};


#endif // __ADN_Automata_GUI_h_
