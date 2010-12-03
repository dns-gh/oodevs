// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#ifndef __ADN_Supply_TrucksDoubleTable_h_
#define __ADN_Supply_TrucksDoubleTable_h_

#include "ADN_Table.h"
#include <boost/noncopyable.hpp>

// =============================================================================
/** @class  ADN_Supply_TrucksDoubleTable
    @brief  ADN_Supply_TrucksDoubleTable
*/
// Created: APE 2005-01-07
// =============================================================================
class ADN_Supply_TrucksDoubleTable : public ADN_Table2
                                   , private boost::noncopyable
{

public:
    //! @name Constructors/Destructor
    //@{
    explicit ADN_Supply_TrucksDoubleTable( QWidget* pParent = 0 );
    virtual ~ADN_Supply_TrucksDoubleTable();
    //@}

    //! @name Operations
    //@{
    virtual void OnContextMenu( int nRow, int nCol, const QPoint& pt );
    //@}
};

#endif // __ADN_Supply_TrucksDoubleTable_h_
