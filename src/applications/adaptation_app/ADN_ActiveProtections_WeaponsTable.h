// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#ifndef __ADN_ActiveProtections_WeaponsTable_h_
#define __ADN_ActiveProtections_WeaponsTable_h_

#include "ADN_CommonGfx.h"
#include "ADN_Equipement_Data.h"

// =============================================================================
//  @class  ADN_ActiveProtections_WeaponsTable
//  @brief  ADN_ActiveProtections_WeaponsTable
// Created: FDS 2010-02-24
// =============================================================================
class ADN_ActiveProtections_WeaponsTable : public ADN_Table3
{
public:
    //! @name Constructors/Destructor
    //@{
             ADN_ActiveProtections_WeaponsTable( const QString& strName, const QString& objectName, ADN_Connector_ABC*& connector, QWidget* pParent = 0 );
    virtual ~ADN_ActiveProtections_WeaponsTable();
    //@}

private:
    //! @name Operations
    //@{
    virtual void AddRow( int row, void* data );
    virtual void OnContextMenu( const QPoint& pt );
    bool Contains( ADN_Equipement_Data::CategoryInfo& category );
    //@}
};

#endif // __ADN_ActiveProtections_WeaponsTable_h_
