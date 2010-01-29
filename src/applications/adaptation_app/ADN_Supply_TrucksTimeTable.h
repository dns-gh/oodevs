// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************
//
// $Created: APE 2005-03-22 $
// $Archive: /MVW_v10/Build/SDK/Adn2/src/ADN_Supply_TrucksTimeTable.h $
// $Author: Ape $
// $Modtime: 20/04/05 17:04 $
// $Revision: 3 $
// $Workfile: ADN_Supply_TrucksTimeTable.h $
//
// *****************************************************************************

#ifndef __ADN_Supply_TrucksTimeTable_h_
#define __ADN_Supply_TrucksTimeTable_h_

#include "ADN_Table.h"


// =============================================================================
/** @class  ADN_Supply_TrucksTimeTable
    @brief  ADN_Supply_TrucksTimeTable
*/
// Created: APE 2005-01-07
// =============================================================================
class ADN_Supply_TrucksTimeTable
    : public ADN_Table2
{
    MT_COPYNOTALLOWED( ADN_Supply_TrucksTimeTable )

public:
    //! @name Constructors/Destructor
    //@{
    ADN_Supply_TrucksTimeTable( QWidget* pParent = 0 );
    virtual ~ADN_Supply_TrucksTimeTable();
    //@}

    //! @name Operations
    //@{
    virtual void OnContextMenu( int nRow, int nCol, const QPoint& pt );
    //@}
};

#endif // __ADN_Supply_TrucksTimeTable_h_
