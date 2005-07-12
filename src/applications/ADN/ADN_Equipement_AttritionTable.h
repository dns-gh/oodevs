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
// $Archive: /MVW_v10/Build/SDK/Adn2/src/ADN_Equipement_AttritionTable.h $
// $Author: Ape $
// $Modtime: 20/04/05 16:50 $
// $Revision: 3 $
// $Workfile: ADN_Equipement_AttritionTable.h $
//
// *****************************************************************************

#ifndef __ADN_Equipement_AttritionTable_h_
#define __ADN_Equipement_AttritionTable_h_

#include "ADN_Table.h"
#include <qapplication.h>

// =============================================================================
/** @class  ADN_Equipement_AttritionTable
    @brief  ADN_Equipement_AttritionTable
    @par    Using example
    @code
    ADN_Equipement_AttritionTable;
    @endcode
*/
// Created: APE 2004-12-29
// =============================================================================
class ADN_Equipement_AttritionTable 
: public ADN_Table2
{
    MT_COPYNOTALLOWED( ADN_Equipement_AttritionTable )

public:
    //! @name Constructors/Destructor
    //@{
     ADN_Equipement_AttritionTable( QWidget* pParent = 0 );
    ~ADN_Equipement_AttritionTable();
    //@}
};

#endif // __ADN_Equipement_AttritionTable_h_
