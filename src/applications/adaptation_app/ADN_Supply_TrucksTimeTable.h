// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ADN_Supply_TrucksTimeTable_h_
#define __ADN_Supply_TrucksTimeTable_h_

#include "ADN_Table.h"
#include <boost/noncopyable.hpp>

// =============================================================================
/** @class  ADN_Supply_TrucksTimeTable
    @brief  ADN_Supply_TrucksTimeTable
*/
// Created: APE 2005-01-07
// =============================================================================
class ADN_Supply_TrucksTimeTable : public ADN_Table2
                                 , private boost::noncopyable
{

public:
    //! @name Constructors/Destructor
    //@{
    explicit ADN_Supply_TrucksTimeTable( QWidget* pParent = 0 );
    virtual ~ADN_Supply_TrucksTimeTable();
    //@}

    //! @name Operations
    //@{
    virtual void OnContextMenu( int nRow, int nCol, const QPoint& pt );
    //@}
};

#endif // __ADN_Supply_TrucksTimeTable_h_
