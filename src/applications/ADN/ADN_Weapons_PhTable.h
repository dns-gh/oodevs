// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: APE 2005-01-07 $
// $Archive: /MVW_v10/Build/SDK/Adn2/src/ADN_Weapons_PhTable.h $
// $Author: Ape $
// $Modtime: 20/04/05 16:50 $
// $Revision: 4 $
// $Workfile: ADN_Weapons_PhTable.h $
//
// *****************************************************************************

#ifndef __ADN_Weapons_PhTable_h_
#define __ADN_Weapons_PhTable_h_

#include "ADN_Table.h"

#include <qapplication.h>


// =============================================================================
/** @class  ADN_Weapons_PhTable
    @brief  ADN_Weapons_PhTable
*/
// Created: APE 2005-01-07
// =============================================================================
class ADN_Weapons_PhTable
: public ADN_Table2
{
    MT_COPYNOTALLOWED( ADN_Weapons_PhTable )

public:
    //! @name Constructors/Destructor
    //@{
     ADN_Weapons_PhTable( QWidget* pParent = 0 );
    ~ADN_Weapons_PhTable();
    //@}

    //! @name Operations
    //@{
    virtual void OnContextMenu( int nRow, int nCol, const QPoint& pt );

    void CreateNewElement();
    void DeleteCurrentElement();
    //@}
};

#endif // __ADN_Weapons_PhTable_h_
