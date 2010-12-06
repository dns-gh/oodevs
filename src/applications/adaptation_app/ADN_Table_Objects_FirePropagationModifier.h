// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: APE 2004-12-29 $
// $Archive: /MVW_v10/Build/SDK/Adn2/src/ADN_Table_Objects_FirePropagationModifier.h $
// $Author: Ape $
// $Modtime: 20/04/05 16:50 $
// $Revision: 3 $
// $Workfile: ADN_Table_Objects_FirePropagationModifier.h $
//
// *****************************************************************************

#ifndef __ADN_Table_Objects_FirePropagationModifier_h_
#define __ADN_Table_Objects_FirePropagationModifier_h_

#include "ADN_Table.h"
#include <qapplication.h>

// =============================================================================
/** @class  ADN_Equipement_AttritionTable
@brief  ADN_Table_Objects_FirePropagationModifier
@par    Using example
@code
ADN_Table_Objects_FirePropagationModifier;
@endcode
*/
// Created: BCI 2010-12-02
// =============================================================================
class ADN_Table_Objects_FirePropagationModifier
    : public ADN_Table2
{
public:
    //! @name Constructors/Destructor
    //@{
    ADN_Table_Objects_FirePropagationModifier( QWidget* pParent = 0 );
    virtual ~ADN_Table_Objects_FirePropagationModifier();
    //@}
};

#endif // __ADN_Table_Objects_FirePropagationModifier_h_
