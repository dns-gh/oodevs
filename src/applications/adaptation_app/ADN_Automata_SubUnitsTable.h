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
#include <boost/noncopyable.hpp>
#include "ADN_Units_Data.h"

// =============================================================================
/** @class  ADN_Automata_SubUnitsTable
    @brief  ADN_Automata_SubUnitsTable
*/
// Created: APE 2005-01-07
// =============================================================================
class ADN_Automata_SubUnitsTable : public ADN_Table3
                                 , private boost::noncopyable
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
    void AddSubItems( const std::string& name );
    //@}

private:
    void AddNewElement( ADN_Units_Data::UnitInfos* );
    void RemoveCurrentElement();

private slots:
    void MenuListItemSelected();

signals:
    void ItemAdded( const std::string& name );
    void ItemRemoved( const std::string& name );

private:
    bool bMenuListItemSelected_;
};

#endif // __ADN_Automata_SubUnitsTable_h_
