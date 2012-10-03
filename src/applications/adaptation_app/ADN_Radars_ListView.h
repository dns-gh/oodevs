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
#include <boost/noncopyable.hpp>

// =============================================================================
/** @class  ADN_Radars_ListView
    @brief  ADN_Radars_ListView
*/
// Created: APE 2005-05-03
// =============================================================================
class ADN_Radars_ListView : public ADN_ListView
                          , private boost::noncopyable
{

public:
    //! @name Constructors/Destructor
    //@{
    explicit ADN_Radars_ListView( QWidget* pParent );
    virtual ~ADN_Radars_ListView();
    //@}

    //! @name Operations
    //@{
    void ConnectItem( bool bConnect );
    void OnContextMenu( const QPoint& pt );
    virtual std::string GetToolTipFor( const QModelIndex& index );
    //@}
};

#endif // __ADN_Radars_ListView_h_
