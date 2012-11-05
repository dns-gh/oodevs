// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ADN_Urban_AttritionTable_h_
#define __ADN_Urban_AttritionTable_h_

#include "ADN_Table.h"

class ADN_Urban_AttritionTable : public ADN_Table3
{
public:
    //! @name Constructors/Destructor
    //@{
             ADN_Urban_AttritionTable( const QString& objectName, ADN_Connector_ABC*& connector, QWidget* pParent = 0 );
    virtual ~ADN_Urban_AttritionTable();
    //@}

    //! @name Operations
    //@{
    virtual void AddRow( int row, void* data );
    //@}
};

#endif // __ADN_Urban_AttritionTable_h_
