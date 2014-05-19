// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2014 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ADN_Equipments_DisasterImpactTable_h_
#define __ADN_Equipments_DisasterImpactTable_h_

#include "ADN_Table.h"

// =============================================================================
/** @class  ADN_Equipments_DisasterImpactTable
    @brief  ADN_Equipments_DisasterImpactTable
*/
// Created: JSR 2014-04-22
// =============================================================================
class ADN_Equipments_DisasterImpactTable : public ADN_Table
{
public:
    //! @name Constructors/Destructor
    //@{
             ADN_Equipments_DisasterImpactTable( const QString& objectName, ADN_Connector_ABC*& connector, QWidget* pParent = 0 );
    virtual ~ADN_Equipments_DisasterImpactTable();
    //@}

    //! @name Operations
    //@{
    virtual void AddRow( int row, void* data );
    virtual void OnContextMenu( const QPoint& pt );
    //@}
};

#endif // __ADN_Equipments_DisasterImpactTable_h_
