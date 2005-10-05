// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: APE 2005-02-10 $
// $Archive: /MVW_v10/Build/SDK/Adn2/src/ADN_Population_ListView.h $
// $Author: Ape $
// $Modtime: 2/03/05 16:58 $
// $Revision: 3 $
// $Workfile: ADN_Population_ListView.h $
//
// *****************************************************************************

#ifndef __ADN_Population_ListView_h_
#define __ADN_Population_ListView_h_

#include "ADN_ListView.h"


// =============================================================================
/** @class  ADN_Population_ListView
    @brief  ADN_Population_ListView
*/
// Created: APE 2005-01-06
// =============================================================================
class ADN_Population_ListView
: public ADN_ListView
{
    MT_COPYNOTALLOWED( ADN_Population_ListView )

public:
    //! @name Constructors/Destructor
    //@{
    ADN_Population_ListView( QWidget* pParent = 0, const char* szName = 0, WFlags f = 0 );
    virtual ~ADN_Population_ListView();
    //@}

    //! @name Operations
    //@{
    void ConnectItem( bool bConnect );
    void OnContextMenu( const QPoint& pt );
    //@}
};

#endif // __ADN_Population_ListView_h_
