// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: APE 2005-01-07 $
// $Archive: /MVW_v10/Build/SDK/Adn2/src/ADN_Weapons_PhSizeListView.h $
// $Author: Ape $
// $Modtime: 1/03/05 14:59 $
// $Revision: 2 $
// $Workfile: ADN_Weapons_PhSizeListView.h $
//
// *****************************************************************************

#ifndef __ADN_Weapons_PhSizeListView_h_
#define __ADN_Weapons_PhSizeListView_h_

#include "ADN_ListView.h"

class GQ_Plot;

// =============================================================================
/** @class  ADN_Weapons_PhSizeListView
    @brief  ADN_Weapons_PhSizeListView
*/
// Created: APE 2005-01-07
// =============================================================================
class ADN_Weapons_PhSizeListView
: public ADN_ListView
{
    MT_COPYNOTALLOWED( ADN_Weapons_PhSizeListView )

public:
    //! @name Constructors/Destructor
    //@{
     ADN_Weapons_PhSizeListView( GQ_Plot& plot, QWidget* pParent = 0, const char* szName = 0, WFlags f = 0 );
    ~ADN_Weapons_PhSizeListView();
    //@}

private:
    //! @name Operations
    //@{
    void ConnectItem( bool bConnect );
    //@}
};

#endif // __ADN_Weapons_PhSizeListView_h_
