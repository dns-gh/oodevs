// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ADN_Breakdowns_ListView_h_
#define __ADN_Breakdowns_ListView_h_

#include "ADN_ListView.h"

// =============================================================================
/** @class  ADN_Breakdowns_ListView
    @brief  ADN_Breakdowns_ListView
*/
// Created: APE 2005-03-18
// =============================================================================
class ADN_Breakdowns_ListView : public ADN_ListView
{

public:
    //! @name Constructors/Destructor
    //@{
    explicit ADN_Breakdowns_ListView( QWidget* pParent );
    virtual ~ADN_Breakdowns_ListView();
    //@}

    //! @name Operations
    //@{
    void ConnectItem( bool bConnect );
    void OnContextMenu( const QPoint& pt );
    virtual std::string GetToolTipFor( const QModelIndex& index );
    //@}
};

#endif // __ADN_Breakdowns_ListView_h_
