// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: APE 2005-03-22 $
// $Archive: /MVW_v10/Build/SDK/Adn2/src/ADN_Supply_TrucksDoubleTable.h $
// $Author: Ape $
// $Modtime: 20/04/05 17:04 $
// $Revision: 3 $
// $Workfile: ADN_Supply_TrucksDoubleTable.h $
//
// *****************************************************************************

#ifndef __ADN_Supply_TrucksDoubleTable_h_
#define __ADN_Supply_TrucksDoubleTable_h_

#include "ADN_Table.h"


// =============================================================================
/** @class  ADN_Supply_TrucksDoubleTable
    @brief  ADN_Supply_TrucksDoubleTable
*/
// Created: APE 2005-01-07
// =============================================================================
class ADN_Supply_TrucksDoubleTable
    : public ADN_Table2
{
    MT_COPYNOTALLOWED( ADN_Supply_TrucksDoubleTable )

public:
    //! @name Constructors/Destructor
    //@{
    ADN_Supply_TrucksDoubleTable( QWidget* pParent = 0 );
    ~ADN_Supply_TrucksDoubleTable();
    //@}

    //! @name Operations
    //@{
    virtual void OnContextMenu( int nRow, int nCol, const QPoint& pt );
    //@}
};

#endif // __ADN_Supply_TrucksDoubleTable_h_
