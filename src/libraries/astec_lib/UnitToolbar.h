// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: APE 2004-09-21 $
// $Archive: /MVW_v10/Build/SDK/Light2/src/UnitToolbar.h $
// $Author: Ape $
// $Modtime: 21/09/04 16:07 $
// $Revision: 1 $
// $Workfile: UnitToolbar.h $
//
// *****************************************************************************

#ifndef __UnitToolbar_h_
#define __UnitToolbar_h_

class Controllers;

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
             UnitToolbar( QMainWindow* pParent, Controllers& controllers );
    virtual ~UnitToolbar();
    //@}

private:
    //! @name Copy / Assignment
    //@{
    UnitToolbar( const UnitToolbar& );
    UnitToolbar& operator=( const UnitToolbar& );
    //@}
};


#endif // __UnitToolbar_h_
