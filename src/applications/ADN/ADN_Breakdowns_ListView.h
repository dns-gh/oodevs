// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: APE 2005-03-18 $
// $Archive: /MVW_v10/Build/SDK/Adn2/src/ADN_Breakdowns_ListView.h $
// $Author: Ape $
// $Modtime: 18/03/05 17:25 $
// $Revision: 1 $
// $Workfile: ADN_Breakdowns_ListView.h $
//
// *****************************************************************************

#ifndef __ADN_Breakdowns_ListView_h_
#define __ADN_Breakdowns_ListView_h_

#include "ADN_ListView.h"
#include "ADN_Breakdowns_Data.h"

// =============================================================================
/** @class  ADN_Breakdowns_ListView
    @brief  ADN_Breakdowns_ListView
*/
// Created: APE 2005-03-18
// =============================================================================
class ADN_Breakdowns_ListView
: public ADN_ListView
{
    MT_COPYNOTALLOWED( ADN_Breakdowns_ListView )

public:
    //! @name Constructors/Destructor
    //@{
    ADN_Breakdowns_ListView( QWidget* pParent = 0, const char* szName = 0, WFlags f = 0 );
    virtual ~ADN_Breakdowns_ListView();
    //@}

    //! @name Operations
    //@{
    void ConnectItem( bool bConnect );
    void OnContextMenu( const QPoint& pt );
    //@}
};

#endif // __ADN_Breakdowns_ListView_h_
