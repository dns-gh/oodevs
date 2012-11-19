// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#ifndef ADN_ThresholdConcentrationsTable_h
#define ADN_ThresholdConcentrationsTable_h

#include "ADN_Table.h"

// =============================================================================
/** @class  ADN_ThresholdConcentrationsTable
    @brief  ADN_ThresholdConcentrationsTable
*/
// Created: LGY 2012-11-13
// =============================================================================
class ADN_ThresholdConcentrationsTable : public ADN_Table
{
public:
    //! @name Constructors/Destructor
    //@{
             ADN_ThresholdConcentrationsTable( const QString& objectName, ADN_Connector_ABC*& connector,
                                               QWidget* pParent /*= 0*/ );
    virtual ~ADN_ThresholdConcentrationsTable();
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

#endif // ADN_ThresholdConcentrationsTable_h
