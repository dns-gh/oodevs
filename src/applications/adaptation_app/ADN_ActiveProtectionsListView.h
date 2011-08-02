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
#include <boost/noncopyable.hpp>

// =============================================================================
//  @class  ADN_ActiveProtectionsListView
//  @brief  ADN_ActiveProtectionsListView
// Created: FDS 10-02-25
// =============================================================================
class ADN_ActiveProtectionsListView : public ADN_ListView
                                    , private boost::noncopyable
{

public:
    //! @name Constructors/Destructor
    //@{
    explicit ADN_ActiveProtectionsListView( QWidget* pParent = 0, const char* szName = 0, Qt::WFlags f = 0 );
    virtual ~ADN_ActiveProtectionsListView();
    //@}

private:
    //! @name Operations
    //@{
    void ConnectItem( bool bConnect );
    void OnContextMenu( const QPoint& pt );
    //@}
};

#endif // __ADN_ActiveProtectionsListView_h_
