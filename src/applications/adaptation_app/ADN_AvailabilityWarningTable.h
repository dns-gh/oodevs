// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ADN_AvailabilityWarningTable_h_
#define __ADN_AvailabilityWarningTable_h_

#include "ADN_Table.h"

// =============================================================================
/** @class  ADN_AvailabilityWarningTable
    @brief  ADN_AvailabilityWarningTable
*/
// Created: SBO 2006-08-03
// =============================================================================
class ADN_AvailabilityWarningTable : public ADN_Table2
{

public:
    //! @name Constructors/Destructor
    //@{
    explicit ADN_AvailabilityWarningTable( QWidget* pParent );
    virtual ~ADN_AvailabilityWarningTable();
    //@}

    //! @name Operations
    //@{
    virtual void OnContextMenu( int nRow, int nCol, const QPoint& pt );
    //@}

private:
    //! @name Helpers
    //@{
    void CreateNewElement();
    void DeleteCurrentElement();
    //@}

};

#endif // __ADN_AvailabilityWarningTable_h_
