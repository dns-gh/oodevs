// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ADN_UrbanModifiersTable_h_
#define __ADN_UrbanModifiersTable_h_

#include "ADN_Table.h"

namespace helpers
{

// =============================================================================
/** @class  ADN_UrbanModifiersTable
    @brief  ADN_UrbanModifiersTable
*/
// Created: SLG 2010-04-13
// =============================================================================
class ADN_UrbanModifiersTable : public ADN_Table
{
    Q_OBJECT

public:
    //! @name Constructors/Destructor
    //@{
             ADN_UrbanModifiersTable( const QString& objectName, ADN_Connector_ABC*& connector, QWidget* pParent = 0 );
    virtual ~ADN_UrbanModifiersTable();
    //@}

public:
    //! @name Operations
    //@{
    virtual void AddRow( int row, void* data );
    //@}
};

} // end namespace helpers

#endif // __ADN_UrbanModifiersTable_h_
