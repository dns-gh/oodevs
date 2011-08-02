// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#ifndef __ADN_ListView_ResourceNetworks_h_
#define __ADN_ListView_ResourceNetworks_h_

#include "ADN_ListView.h"
#include <boost/noncopyable.hpp>

// =============================================================================
/** @class  ADN_ListView_ResourceNetworks
    @brief  ADN_ListView_ResourceNetworks
*/
// Created: JSR 2010-09-13
// =============================================================================
class ADN_ListView_ResourceNetworks : public ADN_ListView
                                    , private boost::noncopyable
{

public:
    //! @name Constructors/Destructor
    //@{
    explicit ADN_ListView_ResourceNetworks( QWidget* pParent = 0, const char* szName = 0, Qt::WFlags f = 0 );
    virtual ~ADN_ListView_ResourceNetworks();
    //@}

    //! @name Operations
    //@{
    void ConnectItem( bool bConnect );
    void OnContextMenu( const QPoint& pt );
    //@}
};

#endif // __ADN_ListView_ResourceNetworks_h_
