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
// $Archive: /MVW_v10/Build/SDK/Adn2/src/ADN_Equipement_UrbanAttritionTable.h $
// $Author: Ape $
// $Modtime: 20/04/05 16:50 $
// $Revision: 3 $
// $Workfile: ADN_Equipement_UrbanAttritionTable.h $
//
// *****************************************************************************

#ifndef __ADN_Equipement_UrbanAttritionTable_h_
#define __ADN_Equipement_UrbanAttritionTable_h_

#include "ADN_Table.h"
#include <qapplication.h>

// =============================================================================
/** @class  ADN_Equipement_AttritionTable
@brief  ADN_Equipement_UrbanAttritionTable
@par    Using example
@code
ADN_Equipement_UrbanAttritionTable;
@endcode
*/
// Created: SLG 2010-04-13
// =============================================================================
class ADN_Equipement_UrbanAttritionTable
    : public ADN_Table2
{
    MT_COPYNOTALLOWED( ADN_Equipement_UrbanAttritionTable )

public:
    //! @name Constructors/Destructor
    //@{
    ADN_Equipement_UrbanAttritionTable( QWidget* pParent = 0 );
    virtual ~ADN_Equipement_UrbanAttritionTable();
    //@}

protected slots:
    virtual void doValueChanged( int row, int col );
};

#endif // __ADN_Equipement_UrbanAttritionTable_h_
