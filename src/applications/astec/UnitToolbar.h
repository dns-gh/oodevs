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

#include "Options.h"

// =============================================================================
/** @class  UnitToolbar
    @brief  UnitToolbar
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
    Options& options_;
    QToolButton* pOldPathsButton_;
};


#endif // __UnitToolbar_h_
