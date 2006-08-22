// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __MapToolbar_h_
#define __MapToolbar_h_

namespace kernel
{
    class Controllers;
}

// =============================================================================
/** @class  MapToolbar
    @brief  MapToolbar
*/
// Created: APE 2004-03-16
// =============================================================================
class MapToolbar : public QToolBar
{

public:
    //! @name Constructors/Destructor
    //@{
             MapToolbar( QMainWindow* pParent, kernel::Controllers& controllers );
    virtual ~MapToolbar();
    //@}
};

#endif // __MapToolbar_h_
