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
#include <boost/noncopyable.hpp>

// =============================================================================
/** @class  ADN_Urban_AttritionTable
    @brief  ADN_Urban_AttritionTable
    @par    Using example
    @code
    ADN_Urban_AttritionTable;
    @endcode
*/
// Created: APE 2004-12-29
// =============================================================================
class ADN_Urban_AttritionTable : public ADN_Table2
                               , private boost::noncopyable
{

public:
    //! @name Constructors/Destructor
    //@{
    explicit ADN_Urban_AttritionTable( QWidget* pParent = 0 );
    virtual ~ADN_Urban_AttritionTable();
    //@}

public:
    virtual void doValueChanged( int row, int col );
};

#endif // __ADN_Urban_AttritionTable_h_
