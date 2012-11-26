// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ADN_Resources_AmmoListView_h_
#define __ADN_Resources_AmmoListView_h_

#include "ADN_ListView.h"
#include <boost/noncopyable.hpp>

// =============================================================================
/** @class  ADN_Resources_AmmoListView
    @brief  ADN_Resources_AmmoListView
*/
// Created: APE 2004-12-29
// =============================================================================
class ADN_Resources_AmmoListView : public ADN_ListView
{

public:
    //! @name Constructors/Destructor
    //@{
    explicit ADN_Resources_AmmoListView( QWidget* pParent );
    virtual ~ADN_Resources_AmmoListView();
    //@}

private:
    //! @name Operations
    //@{
    void        ConnectItem  ( bool           bConnect );
    void        OnContextMenu( const QPoint&  pt       );
    std::string GetToolTipFor( const QModelIndex& index );
    //@}
};

#endif // __ADN_Resources_AmmoListView_h_
