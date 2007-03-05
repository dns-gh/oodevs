// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __DisplayToolbar_h_
#define __DisplayToolbar_h_

namespace kernel
{
    class Controllers;
}

// =============================================================================
/** @class  DisplayToolbar
    @brief  DisplayToolbar
*/
// Created: SBO 2007-03-05
// =============================================================================
class DisplayToolbar : public QToolBar
{

public:
    //! @name Constructors/Destructor
    //@{
             DisplayToolbar( QMainWindow* pParent, kernel::Controllers& controllers );
    virtual ~DisplayToolbar();
    //@}
};

#endif // __DisplayToolbar_h_
