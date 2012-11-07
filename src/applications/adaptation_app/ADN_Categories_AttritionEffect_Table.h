// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ADN_Categories_AttritionEffect_Table_h_
#define __ADN_Categories_AttritionEffect_Table_h_

#include "ADN_Table.h"

class ADN_Categories_AttritionEffect_Table : public ADN_Table
{
public:
    //! @name Constructors/Destructor
    //@{
             ADN_Categories_AttritionEffect_Table( const QString& objectName, ADN_Connector_ABC*& connector, QWidget* pParent = 0 );
    virtual ~ADN_Categories_AttritionEffect_Table();
    //@}

    //! @name Operations
    //@{
    virtual void AddRow( int row, void* data );
    //@}

protected:
    //! @name Slots
    //@{
    virtual void dataChanged( const QModelIndex& topLeft, const QModelIndex& bottomRight );
    //@}
};

#endif // __ADN_Categories_AttritionEffect_Table_h_
