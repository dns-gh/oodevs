// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef _ADN_Schedule_Table_h
#define _ADN_Schedule_Table_h

#include "ADN_Table.h"

// =============================================================================
/** @class  ADN_Schedule_Table
    @brief  ADN schedule table
*/
// Created: ABR 2012-11-05
// =============================================================================
class ADN_Schedule_Table : public ADN_Table
{
public:
    //! @name Constructors/Destructor
    //@{
             ADN_Schedule_Table( const QString& objectName, ADN_Connector_ABC*& connector, QWidget* pParent );
    virtual ~ADN_Schedule_Table();
    //@}

protected:
    //! @name Operations
    //@{
    virtual void AddRow( int row, void* data );
    virtual void OnContextMenu( const QPoint& pt );
    //@}

private:
    //! @name Member data
    //@{
    QStringList days_;
    //@}
};

#endif // _ADN_Schedule_Table_h
