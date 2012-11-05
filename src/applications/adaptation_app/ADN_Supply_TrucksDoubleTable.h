// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
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
class ADN_Supply_TrucksDoubleTable : public ADN_Table3
                                   , private boost::noncopyable
{

public:
    //! @name Constructors/Destructor
    //@{
    explicit ADN_Supply_TrucksDoubleTable( const QString& objectName, ADN_Ref_ABC& vector, QWidget* pParent = 0 );
    virtual ~ADN_Supply_TrucksDoubleTable();
    //@}

    //! @name Operations
    //@{
    virtual void OnContextMenu( const QPoint& pt );
    virtual void AddRow( int row, void* data );
    //@}
};

#endif // __ADN_Supply_TrucksDoubleTable_h_
