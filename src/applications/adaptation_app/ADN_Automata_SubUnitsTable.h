// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: APE 2005-02-10 $
// $Archive: /MVW_v10/Build/SDK/ADN2/src/ADN_Automata_SubUnitsTable.h $
// $Author: Nld $
// $Modtime: 9/05/05 12:13 $
// $Revision: 5 $
// $Workfile: ADN_Automata_SubUnitsTable.h $
//
// *****************************************************************************

#ifndef __ADN_Automata_SubUnitsTable_h_
#define __ADN_Automata_SubUnitsTable_h_

#include "ADN_Table.h"
#include <qapplication.h>

// =============================================================================
/** @class  ADN_Automata_SubUnitsTable
    @brief  ADN_Automata_SubUnitsTable
*/
// Created: APE 2005-01-07
// =============================================================================
class ADN_Automata_SubUnitsTable
: public ADN_Table2
{
    Q_OBJECT
    MT_COPYNOTALLOWED( ADN_Automata_SubUnitsTable )

public:
    //! @name Constructors/Destructor
    //@{
    ADN_Automata_SubUnitsTable( QWidget* pParent = 0 );
    ~ADN_Automata_SubUnitsTable();
    //@}

    //! @name Operations
    //@{
    virtual void OnContextMenu( int nRow, int nCol, const QPoint& pt );
    //@}

private:
    void AddNewElement( int n );
    void RemoveCurrentElement();

private slots:
    void MenuListItemSelected();

private:
    bool bMenuListItemSelected_;
};

#endif // __ADN_Automata_SubUnitsTable_h_
