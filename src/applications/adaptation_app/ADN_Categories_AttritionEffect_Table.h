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

// =============================================================================
/** @class  ADN_Categories_AttritionEffect_Table
    @brief  ADN_Categories_AttritionEffect_Table
*/
// Created: SBO 2006-07-28
// =============================================================================
class ADN_Categories_AttritionEffect_Table
    : public ADN_Table2
{
        Q_OBJECT;

public:
    //! @name Constructors/Destructor
    //@{
             ADN_Categories_AttritionEffect_Table( QWidget* pParent = 0 );
    virtual ~ADN_Categories_AttritionEffect_Table();
    //@}

protected slots:
    //! @name slots
    //@{
    virtual void doValueChanged( int row, int col );
    //@}
};

#endif // __ADN_Categories_AttritionEffect_Table_h_
