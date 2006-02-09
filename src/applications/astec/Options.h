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
// $Archive: /MVW_v10/Build/SDK/Light2/src/Options.h $
// $Author: Age $
// $Modtime: 16/05/05 16:39 $
// $Revision: 10 $
// $Workfile: Options.h $
//
// *****************************************************************************

#ifndef __Options_h_
#define __Options_h_

#ifdef __GNUG__
#   pragma interface
#endif

#include "Types.h"

class MT_Settings;


// =============================================================================
/** @class  Options
    @brief  Contains all of the gui's options.
*/
// Created: APE 2004-03-16
// =============================================================================
class Options
{
    MT_COPYNOTALLOWED( Options );

public:
    enum E_State
    {
        eOn,
        eOff,
        eAuto,
        eAll = eOn,
        eNone = eOff,
        eSel = eAuto
    };

    enum
    {
        eController = -1
    };

public:
    //! @name Constructors/Destructor
    //@{
     Options();
    ~Options();
    //@}

    //! @name Read/Write operations
    //@{
    void WriteSettings( MT_Settings& settings );
    void ReadSettings( MT_Settings& settings );
    //@}

public:
    //! @name Public data for easy access
    //@{
    E_State nDrawSmallText_;
    E_State nDrawLargeText_;
    E_State nDrawDetails_;
    E_State nDrawTacticalLines_;
    E_State nDrawVisionLines_;
    E_State nDrawVisionCones_;
    E_State nDrawVisionSurfaces_;
    E_State nDrawPaths_;
    bool bDrawOldPaths_;
    bool bDisplayWeather_;
    E_State nDisplayLogLinks_;
    E_State nDisplayMissingLogLinks_;
    bool    bDisplayRealTimeLog_;
    bool    bDisplayHoveredInfo_;
    bool    bDisplayRConMap_;
    bool    bDisplayMessagesOnMap_;
    bool    bDisplayTracesOnMap_;
    bool    bDisplayIdentificationLevelOnMap_;
    bool    bDisplayOnlySubscribedAgentsRC_;

    MT_Float rGridStep_;

    int nPlayedTeam_;       // Team index or eController for admin view.
    bool bControllerMode_;  // Access to magic actions and team switching.

//$$$ Not displayed in the options toolbar.

    int nFontSize_;

    bool bOpenTreeToItem_;
    bool bSaveLoadTacticalLines_;

    bool bDrawObjetIcons_;

    bool b3D_;
    bool bNoList_;
    //@}
};

#endif // __Options_h_
