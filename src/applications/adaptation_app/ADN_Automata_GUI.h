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
class UnitsFilter;

// =============================================================================
/** @class  ADN_Automata_GUI
    @brief  ADN_Automata_GUI
*/
// Created: APE 2004-12-06
// =============================================================================
class ADN_Automata_GUI : public ADN_GUI_ABC
{

    Q_OBJECT

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

    ADN_Table*    CreateAutomataCompositionsTable  ();
    ADN_ListView* CreateAutomataLogTable           ();
    ADN_ListView* CreateAutomataLogTablePerDotation();
    //@}

private slots:
    //! @name Slots
    //@{
    void OnItemAdded( const std::string& name );
    void OnItemRemoved( const std::string& name );
    //@}

private:
    //! @name Member data
    //@{
    ADN_Automata_Data& data_;
    ADN_Automata_ListView* pAutomataList_;
    UnitsFilter* pFilter_;
    //@}
};


#endif // __ADN_Automata_GUI_h_
