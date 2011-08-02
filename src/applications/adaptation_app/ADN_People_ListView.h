// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ADN_People_ListView_h_
#define __ADN_People_ListView_h_

#include "ADN_ListView.h"

// =============================================================================
/** @class  ADN_People_ListView
    @brief  ADN_People_ListView
*/
// Created: SLG 2010-11-23
// =============================================================================
class ADN_People_ListView : public ADN_ListView
{
public:
    //! @name Constructors/Destructor
    //@{
    explicit ADN_People_ListView( QWidget* pParent = 0, const char* szName = 0, Qt::WFlags f = 0 );
    virtual ~ADN_People_ListView();
    //@}

    //! @name Operations
    //@{
    void ConnectItem( bool bConnect );
    void OnContextMenu( const QPoint& pt );
    //@}
};

#endif // __ADN_People_ListView_h_
