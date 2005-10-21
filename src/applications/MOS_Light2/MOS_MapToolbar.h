// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: APE 2004-03-16 $
// $Archive: /MVW_v10/Build/SDK/MOS_Light2/src/MOS_MapToolbar.h $
// $Author: Age $
// $Modtime: 13/05/05 17:27 $
// $Revision: 3 $
// $Workfile: MOS_MapToolbar.h $
//
// *****************************************************************************

#ifndef __MOS_MapToolbar_h_
#define __MOS_MapToolbar_h_

#ifdef __GNUG__
#   pragma interface
#endif

#include "MOS_Options.h"

class MT_ToolListButton;
class QToolButton;

class MOS_Agent;
class MOS_RC;
class MOS_Report_ABC;
class MOS_SelectedElement;


// =============================================================================
/** @class  MOS_MapToolbar
    @brief  MOS_MapToolbar
    @par    Using example
    @code
    MOS_MapToolbar;
    @endcode
*/
// Created: APE 2004-03-16
// =============================================================================
class MOS_MapToolbar : public QToolBar
{
    Q_OBJECT;
    MT_COPYNOTALLOWED( MOS_MapToolbar );

public:
    //! @name Constructors/Destructor
    //@{
     MOS_MapToolbar( QMainWindow* pParent );
    ~MOS_MapToolbar();
    //@}

private:
    //! @name Helpers
    //@{
    void FillButton( MT_ToolListButton& button, MOS_Options::E_State nState );
    //@}

private slots:
    //! @name Slots
    //@{
    void OnSmallText( float rValue );
    void OnLargeText( float rValue );
    void OnDetails( float rValue );
    void OnTacticalLines( float rValue );
    void OnGrid( float rValue );
    void OnWeatherToggled();
    void On3DToggle();
    //@}

private:
    QToolButton* pWeatherButton_;
    QToolButton* p3DButton_;
};

#endif // __MOS_MapToolbar_h_
