// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: APE 2004-12-29 $
// $Archive: /MVW_v10/Build/SDK/Adn2/src/ADN_Equipement_AmmoListView.h $
// $Author: Ape $
// $Modtime: 1/03/05 11:37 $
// $Revision: 2 $
// $Workfile: ADN_Equipement_AmmoListView.h $
//
// *****************************************************************************

#ifndef __ADN_Equipement_AmmoListView_h_
#define __ADN_Equipement_AmmoListView_h_

#include "ADN_ListView.h"


// =============================================================================
/** @class  ADN_Equipement_AmmoListView
    @brief  ADN_Equipement_AmmoListView
*/
// Created: APE 2004-12-29
// =============================================================================
class ADN_Equipement_AmmoListView
: public ADN_ListView
{
    MT_COPYNOTALLOWED( ADN_Equipement_AmmoListView )

public:
    //! @name Constructors/Destructor
    //@{
     ADN_Equipement_AmmoListView( QWidget* pParent = 0, const char* szName = 0, WFlags f = 0 );
    ~ADN_Equipement_AmmoListView();
    //@}

private:
    //! @name Operations
    //@{
    void        ConnectItem  ( bool           bConnect );
    void        OnContextMenu( const QPoint&  pt       );
    std::string GetToolTipFor( QListViewItem& item     );
    //@}
};

#endif // __ADN_Equipement_AmmoListView_h_
