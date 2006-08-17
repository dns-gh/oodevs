// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: APE 2005-03-21 $
// $Archive: /MVW_v10/Build/SDK/Adn2/src/ADN_KnowledgeGroups_GUI.h $
// $Author: Ape $
// $Modtime: 21/03/05 16:34 $
// $Revision: 1 $
// $Workfile: ADN_KnowledgeGroups_GUI.h $
//
// *****************************************************************************

#ifndef __ADN_KnowledgeGroups_GUI_h_
#define __ADN_KnowledgeGroups_GUI_h_

#include "ADN_GUI_ABC.h"

class ADN_KnowledgeGroups_Data;


// =============================================================================
/** @class  ADN_KnowledgeGroups_GUI
    @brief  ADN_KnowledgeGroups_GUI
*/
// Created: APE 2004-12-06
// =============================================================================
class ADN_KnowledgeGroups_GUI : public ADN_GUI_ABC
{
    MT_COPYNOTALLOWED( ADN_KnowledgeGroups_GUI )

public:
    enum E_GuiElements
    {
        eName,
        eAgentMaxLifetime,
        eAgentMaxDistance,
        eAgentHasInterpolationTime,
        eAgentInterpolationTime,
        ePopulationMaxLifetime,
        eNbrGuiElements
    };

public:
    //! @name Constructors/Destructor
    //@{
     ADN_KnowledgeGroups_GUI( ADN_KnowledgeGroups_Data& data );
    ~ADN_KnowledgeGroups_GUI();
    //@}

    //! @name Operations
    //@{
    void Build();
    //@}

private:
    //! @name Member data
    //@{
    ADN_KnowledgeGroups_Data& data_;
    //@}
};

#endif // __ADN_KnowledgeGroups_GUI_h_
