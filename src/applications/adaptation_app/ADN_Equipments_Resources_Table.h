// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#ifndef __ADN_Equipments_Resources_Table_h_
#define __ADN_Equipments_Resources_Table_h_

#include "ADN_Table.h"

// =============================================================================
/** @class  ADN_Equipments_Resources_Table
    @brief  ADN_Equipments_Resources_Table
*/
// Created: ABR 2012-12-03
// =============================================================================
class ADN_Equipments_Resources_Table : public ADN_Table
{
public:
    //! @name Constructors/Destructor
    //@{
             ADN_Equipments_Resources_Table( const QString& objectName, ADN_Connector_ABC*& connector, QWidget* parent = 0 );
    virtual ~ADN_Equipments_Resources_Table();
    //@}

private:
    //! @name Helpers
    //@{
    virtual void AddRow( int row, void* data );
    bool Contains( void* category );
    virtual void OnContextMenu( const QPoint& pt );
};

#endif // __ADN_Equipments_Resources_Table_h_
