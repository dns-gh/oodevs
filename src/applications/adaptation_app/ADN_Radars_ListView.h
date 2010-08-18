// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
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
class ADN_Radars_ListView : public ADN_ListView
{
    MT_COPYNOTALLOWED( ADN_Radars_ListView )

public:
    //! @name Constructors/Destructor
    //@{
             ADN_Radars_ListView( QWidget* pParent = 0, const char* szName = 0, WFlags f = 0 );
    virtual ~ADN_Radars_ListView();
    //@}

    //! @name Operations
    //@{
    void ConnectItem( bool bConnect );
    void OnContextMenu( const QPoint& pt );
    //@}
};

#endif // __ADN_Radars_ListView_h_
