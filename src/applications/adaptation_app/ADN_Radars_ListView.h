// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: APE 2005-05-03 $
// $Archive: /MVW_v10/Build/SDK/Adn2/src/ADN_Radars_ListView.h $
// $Author: Nld $
// $Modtime: 3/05/05 10:11 $
// $Revision: 1 $
// $Workfile: ADN_Radars_ListView.h $
//
// *****************************************************************************

#ifndef __ADN_Radars_ListView_h_
#define __ADN_Radars_ListView_h_

#include "ADN_ListView.h"

// =============================================================================
/** @class  ADN_Radars_ListView
    @brief  ADN_Radars_ListView
*/
// Created: APE 2005-05-03
// =============================================================================
class ADN_Radars_ListView
    : public ADN_ListView
{
    MT_COPYNOTALLOWED( ADN_Radars_ListView )

public:
    //! @name Constructors/Destructor
    //@{
     ADN_Radars_ListView( QWidget* pParent = 0, const char* szName = 0, WFlags f = 0 );
    ~ADN_Radars_ListView();
    //@}

    //! @name Operations
    //@{
    void ConnectItem( bool bConnect );
    void OnContextMenu( const QPoint& pt );
    //@}
};

#endif // __ADN_Radars_ListView_h_
