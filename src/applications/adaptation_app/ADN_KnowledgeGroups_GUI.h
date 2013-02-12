// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
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
    Q_OBJECT

public:
    enum E_GuiElements
    {
        eName,
        eCommunicationDelay,
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
    explicit ADN_KnowledgeGroups_GUI( ADN_KnowledgeGroups_Data& data );
    virtual ~ADN_KnowledgeGroups_GUI();
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
