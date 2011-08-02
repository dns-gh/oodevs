// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ADN_Weapons_PhTable_h_
#define __ADN_Weapons_PhTable_h_

#include "ADN_Table.h"
#include <QtGui/qapplication.h>
#include <boost/noncopyable.hpp>

// =============================================================================
/** @class  ADN_Weapons_PhTable
    @brief  ADN_Weapons_PhTable
*/
// Created: APE 2005-01-07
// =============================================================================
class ADN_Weapons_PhTable : public ADN_Table2
                          , private boost::noncopyable
{

public:
    //! @name Constructors/Destructor
    //@{
    explicit ADN_Weapons_PhTable( QWidget* pParent = 0 );
    virtual ~ADN_Weapons_PhTable();
    //@}

    //! @name Operations
    //@{
    virtual void OnContextMenu( int nRow, int nCol, const QPoint& pt );
    virtual void doValueChanged( int row, int col );

    void CreateNewElement();
    void DeleteCurrentElement();
    //@}
};

#endif // __ADN_Weapons_PhTable_h_
