// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#ifndef __ADN_ActiveProtectionsListView_h_
#define __ADN_ActiveProtectionsListView_h_

#include "ADN_ListView.h"

// =============================================================================
//  @class  ADN_ActiveProtectionsListView
//  @brief  ADN_ActiveProtectionsListView
// Created: FDS 10-02-25
// =============================================================================
class ADN_ActiveProtectionsListView : public ADN_ListView
{

public:
    //! @name Constructors/Destructor
    //@{
    explicit ADN_ActiveProtectionsListView( QWidget* pParent );
    virtual ~ADN_ActiveProtectionsListView();
    //@}

private:
    //! @name Operations
    //@{
    void ConnectItem( bool bConnect );
    void OnContextMenu( const QPoint& pt );
    std::string GetToolTipFor( const QModelIndex& index );
    //@}
};

#endif // __ADN_ActiveProtectionsListView_h_
