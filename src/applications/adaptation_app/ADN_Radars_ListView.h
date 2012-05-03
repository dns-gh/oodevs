// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Math�matiques Appliqu�es SA (MASA)
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
    explicit ADN_Radars_ListView( QWidget* pParent = 0, const char* szName = 0, Qt::WFlags f = 0 );
    virtual ~ADN_Radars_ListView();
    //@}

    //! @name Operations
    //@{
    void ConnectItem( bool bConnect );
    void OnContextMenu( const QPoint& pt );
    virtual std::string GetToolTipFor( Q3ListViewItem& item );
    //@}
};

#endif // __ADN_Radars_ListView_h_
