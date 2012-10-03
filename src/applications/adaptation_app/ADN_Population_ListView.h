// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#ifndef __ADN_Population_ListView_h_
#define __ADN_Population_ListView_h_

#include "ADN_ListView.h"
#include <boost/noncopyable.hpp>

// =============================================================================
/** @class  ADN_Population_ListView
    @brief  ADN_Population_ListView
*/
// Created: APE 2005-01-06
// =============================================================================
class ADN_Population_ListView : public ADN_ListView
                              , private boost::noncopyable
{

public:
    //! @name Constructors/Destructor
    //@{
    explicit ADN_Population_ListView( QWidget* pParent );
    virtual ~ADN_Population_ListView();
    //@}

    //! @name Operations
    //@{
    void ConnectItem( bool bConnect );
    void OnContextMenu( const QPoint& pt );
    std::string GetToolTipFor( const QModelIndex& index );
    //@}
};

#endif // __ADN_Population_ListView_h_
