// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __UnitToolbar_h_
#define __UnitToolbar_h_

namespace kernel
{
    class Controllers;
}

// =============================================================================
/** @class  UnitToolbar
    @brief  UnitToolbar
*/
// Created: APE 2004-09-21
// =============================================================================
class UnitToolbar : public QToolBar
{
public:
    //! @name Constructors/Destructor
    //@{
             UnitToolbar( QMainWindow* pParent, kernel::Controllers& controllers );
    virtual ~UnitToolbar();
    //@}
};


#endif // __UnitToolbar_h_
