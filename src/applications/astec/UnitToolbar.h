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

#ifdef __GNUG__
#   pragma interface
#endif

#include "Options.h"


// =============================================================================
/** @class  UnitToolbar
    @brief  UnitToolbar
    @par    Using example
    @code
    UnitToolbar;
    @endcode
*/
// Created: APE 2004-09-21
// =============================================================================
class UnitToolbar : public QToolBar
{
    Q_OBJECT;
    MT_COPYNOTALLOWED( UnitToolbar );

public:
    //! @name Constructors/Destructor
    //@{
     UnitToolbar( QMainWindow* pParent );
    ~UnitToolbar();
    //@}

private slots:
    //! @name Slots
    //@{
    void OnVisionLines( float rValue );
    void OnVisionCones( float rValue );
    void OnVisionSurfaces( float rValue );
    void OnPaths( float rValue );

    void OnOldPathsToggled();
    //@}

private:
    //! @name Helpers
    //@{
    void FillButton( MT_ToolListButton& button, Options::E_State nState );
    //@}

private:
    QToolButton* pOldPathsButton_;
};


#endif // __UnitToolbar_h_
