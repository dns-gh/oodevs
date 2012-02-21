// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2003 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ADN_Automata_Wizard_h_
#define __ADN_Automata_Wizard_h_

#include "ADN_Automata_Data.h"
#include "ADN_Wizard_ABC.h"

typedef ADN_Wizard_ABC<ADN_Automata_Data::AutomatonInfos> ADN_Wizard_ABC_ADN_Automata_Data_AutomatonInfos;

// =============================================================================
/** @class  ADN_Automata_Wizard
    @brief  ADN_Automata_Wizard
*/
// Created: APE 2005-02-14
// =============================================================================
class ADN_Automata_Wizard : public ADN_Wizard_ABC_ADN_Automata_Data_AutomatonInfos
{

public:
    //! @name Constructors/Destructor
    //@{
    explicit ADN_Automata_Wizard( QWidget* pParent = 0, const char* szName = 0 );
    virtual ~ADN_Automata_Wizard();
    //@}
};

#endif // __ADN_Automata_Wizard_h_
