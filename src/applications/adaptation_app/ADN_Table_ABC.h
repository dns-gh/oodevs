// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2014 MASA Group
//
// *****************************************************************************

#ifndef __ADN_Table_ABC_h_
#define __ADN_Table_ABC_h_

#include "ADN_Gfx_ABC.h"

class ADN_Table_ABC : public ADN_Gfx_ABC
{
public:
    //! @name Constructors / Destructor
    //@{
             ADN_Table_ABC() {}
    virtual ~ADN_Table_ABC() {}
    //@}

    virtual void SetEnabled( bool enable ) = 0;
    virtual void setNumRows( int rows ) = 0;
    virtual void RemoveItem( void* item ) = 0;
    virtual void AddRow( int nRow, void* pObj ) = 0;
    virtual void CheckValidity( int row = -1, int col = -1 ) = 0;
};

#endif // __ADN_Table_ABC_h_
