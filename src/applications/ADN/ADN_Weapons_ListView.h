// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: APE 2005-01-06 $
// $Archive: /MVW_v10/Build/SDK/Adn2/src/ADN_Weapons_ListView.h $
// $Author: Ape $
// $Modtime: 25/04/05 11:14 $
// $Revision: 4 $
// $Workfile: ADN_Weapons_ListView.h $
//
// *****************************************************************************

#ifndef __ADN_Weapons_ListView_h_
#define __ADN_Weapons_ListView_h_

#include "ADN_ListView.h"


// =============================================================================
/** @class  ADN_Weapons_ListView
    @brief  ADN_Weapons_ListView
*/
// Created: APE 2005-01-06
// =============================================================================
class ADN_Weapons_ListView
: public ADN_ListView
{
    MT_COPYNOTALLOWED( ADN_Weapons_ListView )

public:
    //! @name Constructors/Destructor
    //@{
     ADN_Weapons_ListView( QWidget* pParent = 0, const char* szName = 0, WFlags f = 0 );
    ~ADN_Weapons_ListView();
    //@}

    //! @name Operations
    //@{
    void ConnectItem( bool bConnect );
    void OnContextMenu( const QPoint& pt );

    std::string GetToolTipFor( QListViewItem& item );
    //@}
};


#endif // __ADN_Weapons_ListView_h_
