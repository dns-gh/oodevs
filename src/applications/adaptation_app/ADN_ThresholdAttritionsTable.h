// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#ifndef ADN_ThresholdAttritionsTable_h
#define ADN_ThresholdAttritionsTable_h

#include "ADN_Table.h"

// =============================================================================
/** @class  ADN_ThresholdAttritionsTable
    @brief  ADN_ThresholdAttritionsTable
*/
// Created: LGY 2012-11-13
// =============================================================================
class ADN_ThresholdAttritionsTable : public ADN_Table
{
public:
    //! @name Constructors/Destructor
    //@{
             ADN_ThresholdAttritionsTable( const QString& objectName, ADN_Connector_ABC*& connector,
                                           QWidget* pParent /*= 0*/ );
    virtual ~ADN_ThresholdAttritionsTable();
    //@}

    //! @name Operations
    //@{
    virtual void AddRow( int row, void* data );
    virtual void OnContextMenu( const QPoint& point );
    //@}

private:
    //! @name Helpers
    //@{
    void AddNewElement();
    void RemoveCurrentElement();
    //@}

private:
    //! @name Member data
    //@{
    //@}
};

#endif // ADN_ThresholdAttritionsTable_h
