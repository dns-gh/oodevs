// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ADN_KnowledgeGroups_Wizard_h_
#define __ADN_KnowledgeGroups_Wizard_h_

#include "ADN_KnowledgeGroups_Data.h"
#include "ADN_Wizard_ABC.h"

typedef ADN_Wizard_ABC< ADN_KnowledgeGroups_Data::GroupInfo > ADN_Wizard_ABC_ADN_KnowledgeGroups_Data_GroupInfo;

// =============================================================================
/** @class  ADN_KnowledgeGroups_Wizard
    @brief  ADN_KnowledgeGroups_Wizard
*/
// Created: LGY 2010-08-18
// =============================================================================
class ADN_KnowledgeGroups_Wizard : public ADN_Wizard_ABC_ADN_KnowledgeGroups_Data_GroupInfo
{

public:
    //! @name Constructors/Destructor
    //@{
    explicit ADN_KnowledgeGroups_Wizard( QWidget* pParent = 0, const char* szName = 0 );
    virtual ~ADN_KnowledgeGroups_Wizard();
    //@}
};

#endif // __ADN_KnowledgeGroups_Wizard_h_
