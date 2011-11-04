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

// =============================================================================
/** @class  ADN_Automata_SubUnitsTable
    @brief  ADN_Automata_SubUnitsTable
*/
// Created: APE 2005-01-07
// =============================================================================
class ADN_Automata_SubUnitsTable : public ADN_Table2
                                 , private boost::noncopyable
{
    Q_OBJECT

public:
    //! @name Constructors/Destructor
    //@{
    explicit ADN_Automata_SubUnitsTable( QWidget* pParent = 0 );
    virtual ~ADN_Automata_SubUnitsTable();
    //@}

    //! @name Operations
    //@{
    virtual void OnContextMenu( int nRow, int nCol, const QPoint& pt );
    void AddSubItems( const std::string& name );
    //@}

private:
    void AddNewElement( int n );
    void RemoveCurrentElement();

private slots:
    void MenuListItemSelected();

signals:
    void AddItem( const std::string& name );
    void RemoveItem( const std::string& name );

private:
    bool bMenuListItemSelected_;
};

#endif // __ADN_Automata_SubUnitsTable_h_
