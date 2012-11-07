// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ADN_Breakdowns_PartsTable_h_
#define __ADN_Breakdowns_PartsTable_h_

#include "ADN_Table.h"
#include "ADN_Equipement_Data.h"

// =============================================================================
/** @class  ADN_Breakdowns_PartsTable
    @brief  ADN_Breakdowns_PartsTable
*/
// Created: APE 2005-01-07
// =============================================================================
class ADN_Breakdowns_PartsTable : public ADN_Table
{

public:
    //! @name Constructors/Destructor
    //@{
             ADN_Breakdowns_PartsTable( const QString& objectName, ADN_Connector_ABC*& connector, QWidget* pParent = 0 );
    virtual ~ADN_Breakdowns_PartsTable();
    //@}

private:
    //! @name Operations
    //@{
    virtual void AddRow( int row, void* data );
    virtual void OnContextMenu( const QPoint& pt );
    bool Contains( ADN_Equipement_Data::CategoryInfo& category );
    //@}
};

#endif // __ADN_Breakdowns_PartsTable_h_
