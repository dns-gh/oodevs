// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ADN_Inhabitants_ListView_h_
#define __ADN_Inhabitants_ListView_h_

#include "ADN_ListView.h"

// =============================================================================
/** @class  ADN_Inhabitants_ListView
    @brief  ADN_Inhabitants_ListView
*/
// Created: SLG 2010-11-23
// =============================================================================
class ADN_Inhabitants_ListView : public ADN_ListView
{
public:
    //! @name Constructors/Destructor
    //@{
    explicit ADN_Inhabitants_ListView( QWidget* pParent );
    virtual ~ADN_Inhabitants_ListView();
    //@}

    //! @name Operations
    //@{
    void ConnectItem( bool bConnect );
    void OnContextMenu( const QPoint& pt );
    //@}
};

#endif // __ADN_Inhabitants_ListView_h_
