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
// $Archive: /MVW_v10/Build/SDK/MOS_Light2/src/MOS_UnitToolbar.h $
// $Author: Ape $
// $Modtime: 21/09/04 16:07 $
// $Revision: 1 $
// $Workfile: MOS_UnitToolbar.h $
//
// *****************************************************************************

#ifndef __MOS_UnitToolbar_h_
#define __MOS_UnitToolbar_h_

#ifdef __GNUG__
#   pragma interface
#endif

#include "MOS_Options.h"


// =============================================================================
/** @class  MOS_UnitToolbar
    @brief  MOS_UnitToolbar
    @par    Using example
    @code
    MOS_UnitToolbar;
    @endcode
*/
// Created: APE 2004-09-21
// =============================================================================
class MOS_UnitToolbar : public QToolBar
{
    Q_OBJECT;
    MT_COPYNOTALLOWED( MOS_UnitToolbar );

public:
    //! @name Constructors/Destructor
    //@{
     MOS_UnitToolbar( QMainWindow* pParent );
    ~MOS_UnitToolbar();
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
    void FillButton( MT_ToolListButton& button, MOS_Options::E_State nState );
    //@}

private:
    QToolButton* pOldPathsButton_;
};


#endif // __MOS_UnitToolbar_h_
