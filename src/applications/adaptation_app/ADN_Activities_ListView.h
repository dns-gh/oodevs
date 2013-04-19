// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2013 MASA Group
//
// *****************************************************************************

#ifndef __ADN_Activities_ListView_h_
#define __ADN_Activities_ListView_h_

#include "ADN_ListView.h"
#include "ADN_Activities_Data.h"

// =============================================================================
/** @class  ADN_Activities_ListView
    @brief  ADN_Activities_ListView
*/
// Created: NPT 2013-04-16
// =============================================================================
class ADN_Activities_ListView : public ADN_ListView
                              , private boost::noncopyable
{

public:
    //! @name Constructors/Destructor
    //@{
    explicit ADN_Activities_ListView( QWidget* pParent = 0, const char* szName = 0, Qt::WFlags f = 0 );
    virtual ~ADN_Activities_ListView();
    //@}

    //! @name Operations
    //@{
    void ConnectItem( bool bConnect );
    void OnContextMenu( const QPoint& pt );
    //@}
};

#endif // __ADN_Activities_ListView_h_
