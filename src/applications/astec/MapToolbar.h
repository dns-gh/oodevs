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
// $Archive: /MVW_v10/Build/SDK/Light2/src/MapToolbar.h $
// $Author: Age $
// $Modtime: 13/05/05 17:27 $
// $Revision: 3 $
// $Workfile: MapToolbar.h $
//
// *****************************************************************************

#ifndef __MapToolbar_h_
#define __MapToolbar_h_

#include "Options.h"

class MT_ToolListButton;
class QToolButton;

class Agent;
class RC;
class Report_ABC;
class SelectedElement;
class Options;

// =============================================================================
/** @class  MapToolbar
    @brief  MapToolbar
    @par    Using example
    @code
    MapToolbar;
    @endcode
*/
// Created: APE 2004-03-16
// =============================================================================
class MapToolbar : public QToolBar
{
    Q_OBJECT;
    MT_COPYNOTALLOWED( MapToolbar );

public:
    //! @name Constructors/Destructor
    //@{
     MapToolbar( QMainWindow* pParent );
    ~MapToolbar();
    //@}

private:
    //! @name Helpers
    //@{
    void FillButton( MT_ToolListButton& button, Options::E_State nState );
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
    Options& options_;

    QToolButton* pWeatherButton_;
    QToolButton* p3DButton_;
};

#endif // __MapToolbar_h_
