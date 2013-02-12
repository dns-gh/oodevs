// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ADN_Automata_SubUnitsTable_h_
#define __ADN_Automata_SubUnitsTable_h_

#include "ADN_Table.h"

// =============================================================================
/** @class  ADN_Automata_SubUnitsTable
    @brief  ADN_Automata_SubUnitsTable
*/
// Created: APE 2005-01-07
// =============================================================================
class ADN_Automata_SubUnitsTable : public ADN_Table
{
    Q_OBJECT

public:
    //! @name Constructors/Destructor
    //@{
    explicit ADN_Automata_SubUnitsTable( const QString& objectName, ADN_Connector_ABC*& connector, QWidget* pParent = 0 );
    virtual ~ADN_Automata_SubUnitsTable();
    //@}

    //! @name Operations
    //@{
    virtual void OnContextMenu( const QPoint& pt );
    virtual void AddRow( int row, void* data );
    virtual void RemoveCurrentElement();
    void AddSubItems( const std::string& name );
    //@}

signals:
    //! @name Signals
    //@{
    void ItemAdded( const std::string& name );
    void ItemRemoved( const std::string& name );
    //@}
};

#endif // __ADN_Automata_SubUnitsTable_h_
