// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ADN_Equipments_ConsumptionsTable_h_
#define __ADN_Equipments_ConsumptionsTable_h_

#include "ADN_Table.h"

// =============================================================================
/** @class  ADN_Equipments_ConsumptionsTable
    @brief  ADN_Equipments_ConsumptionsTable
*/
// Created: APE 2005-01-25
// =============================================================================
class ADN_Equipments_ConsumptionsTable : public ADN_Table
{
public:
    //! @name Constructors/Destructor
    //@{
             ADN_Equipments_ConsumptionsTable( const QString& objectName, ADN_Connector_ABC*& connector, QWidget* pParent = 0 );
    virtual ~ADN_Equipments_ConsumptionsTable();
    //@}

    //! @name Operations
    //@{
    virtual void AddRow( int row, void* data );
    //@}
};

#endif // __ADN_Equipments_ConsumptionsTable_h_
