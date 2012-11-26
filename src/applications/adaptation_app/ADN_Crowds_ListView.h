// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ADN_Crowds_ListView_h_
#define __ADN_Crowds_ListView_h_

#include "ADN_ListView.h"

// =============================================================================
/** @class  ADN_Crowds_ListView
    @brief  ADN_Crowds_ListView
*/
// Created: APE 2005-01-06
// =============================================================================
class ADN_Crowds_ListView : public ADN_ListView
{

public:
    //! @name Constructors/Destructor
    //@{
    explicit ADN_Crowds_ListView( QWidget* pParent );
    virtual ~ADN_Crowds_ListView();
    //@}

    //! @name Operations
    //@{
    void ConnectItem( bool bConnect );
    void OnContextMenu( const QPoint& pt );
    std::string GetToolTipFor( const QModelIndex& index );
    //@}
};

#endif // __ADN_Crowds_ListView_h_
