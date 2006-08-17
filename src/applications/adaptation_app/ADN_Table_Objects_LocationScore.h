// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: AGN 2004-05-24 $
// $Archive: /MVW_v10/Build/SDK/Adn2/src/ADN_Table_Objects_LocationScore.h $
// $Author: Ape $
// $Modtime: 20/04/05 16:50 $
// $Revision: 5 $
// $Workfile: ADN_Table_Objects_LocationScore.h $
//
// *****************************************************************************

#ifndef __ADN_Table_Objects_LocationScore_h_
#define __ADN_Table_Objects_LocationScore_h_

#include "ADN_CommonGfx.h"

// =============================================================================
/** @class  ADN_Table_Objects_LocationScore
    @brief  ADN_Table_Objects_LocationScore
*/
// Created: AGN 2004-05-24
// =============================================================================
class ADN_Table_Objects_LocationScore
: public ADN_Table2
{
public:
    explicit ADN_Table_Objects_LocationScore(QWidget * parent = 0 );
    virtual ~ADN_Table_Objects_LocationScore();

    virtual void OnContextMenu(int row,int col,const QPoint& pt);

    void AddNewElement( int n );
    void RemoveCurrentElement();
    bool Contains( const std::string& strMunitionName );
};


#endif // __ADN_Table_Objects_LocationScore_h_
