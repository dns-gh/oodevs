// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ADN_Weapons_PhSizeListView_h_
#define __ADN_Weapons_PhSizeListView_h_

#include "ADN_ListView.h"
#include <boost/noncopyable.hpp>

namespace gui
{
    class GQ_Plot;
}

// =============================================================================
/** @class  ADN_Weapons_PhSizeListView
    @brief  ADN_Weapons_PhSizeListView
*/
// Created: APE 2005-01-07
// =============================================================================
class ADN_Weapons_PhSizeListView : public ADN_ListView
                                 , private boost::noncopyable
{

public:
    //! @name Constructors/Destructor
    //@{
    explicit ADN_Weapons_PhSizeListView( gui::GQ_Plot& plot, const std::map< void*, unsigned int >& userIds );
    virtual ~ADN_Weapons_PhSizeListView();
    //@}

private:
    //! @name Operations
    //@{
    void ConnectItem( bool bConnect );
    //@}
};

#endif // __ADN_Weapons_PhSizeListView_h_
