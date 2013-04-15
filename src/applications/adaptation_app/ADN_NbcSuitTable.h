// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#ifndef ADN_NbcSuitTable_h
#define ADN_NbcSuitTable_h

#include "ADN_Table.h"

// =============================================================================
/** @class  ADN_NbcSuitTable
    @brief  ADN_NbcSuitTable
*/
// Created: LGY 2012-11-23
// =============================================================================
class ADN_NbcSuitTable : public ADN_Table
{
public:
    //! @name Constructors/Destructor
    //@{
             ADN_NbcSuitTable( const QString& objectName, ADN_Connector_ABC*& connector,
                               QWidget* pParent /* = 0 */ );
    virtual ~ADN_NbcSuitTable();
    //@}

    //! @name Operations
    //@{
    virtual void AddRow( int row, void* data );
    //@}
};

#endif // ADN_ADN_NbcSuitTable_h
