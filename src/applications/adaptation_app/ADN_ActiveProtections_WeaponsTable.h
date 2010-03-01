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

// =============================================================================
//  @class  ADN_ActiveProtections_WeaponsTable
//  @brief  ADN_ActiveProtections_WeaponsTable
// Created: FDS 2010-02-24
// =============================================================================
class ADN_ActiveProtections_WeaponsTable
: public ADN_Table2
{
    Q_OBJECT

public:
    explicit ADN_ActiveProtections_WeaponsTable( const std::string& strName, QWidget* pParent = 0 );
    virtual ~ADN_ActiveProtections_WeaponsTable();

public slots:
    void OnModified();

private:
    virtual void OnContextMenu( int nRow, int nCol, const QPoint& pt );
};

#endif // __ADN_ActiveProtections_WeaponsTable_h_
