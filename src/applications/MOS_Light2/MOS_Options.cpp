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
// $Archive: /MVW_v10/Build/SDK/MOS_Light2/src/MOS_Options.cpp $
// $Author: Age $
// $Modtime: 16/05/05 16:59 $
// $Revision: 10 $
// $Workfile: MOS_Options.cpp $
//
// *****************************************************************************

#ifdef __GNUG__
#   pragma implementation
#endif

#include "MOS_Light2_pch.h"
#include "MOS_Options.h"

#include "MT_Settings.h"
#include "MT/MT_IO/MT_CommandLine.h"

// -----------------------------------------------------------------------------
// Name: MOS_Options constructor
// Created: APE 2004-03-16
// -----------------------------------------------------------------------------
MOS_Options::MOS_Options()
    : nDrawSmallText_     ( eAuto )
    , nDrawLargeText_     ( eAuto )
    , nDrawDetails_       ( eAuto )
    , nDrawTacticalLines_ ( eOn )
    , nDrawVisionLines_   ( eAuto )
    , nDrawVisionCones_   ( eOff )
    , nDrawVisionSurfaces_( eOff )
    , nDrawPaths_         ( eOn )
    , bDrawOldPaths_      ( false )
    , bDisplayWeather_    ( false )
    , bDisplayLoglinks_   ( false )
    , bDisplayAllLogLinks_( false )
    , rGridStep_          ( 1.0 )
    , bOpenTreeToItem_    ( true )
    , nPlayedTeam_        ( eController )
    , bControllerMode_    ( true )
    , nFontSize_          ( 10 )
    , bSaveLoadTacticalLines_ ( true )
    , b3D_( false )
    , bNoList_( true )
{
    MT_CommandLine arguments( qApp->argc(), qApp->argv() );
    bNoList_ = arguments.IsOptionSet( "-nolist" );
}


// -----------------------------------------------------------------------------
// Name: MOS_Options destructor
// Created: APE 2004-03-16
// -----------------------------------------------------------------------------
MOS_Options::~MOS_Options()
{
}


// -----------------------------------------------------------------------------
// Name: MOS_Options::WriteSettings
/** @param  settings 
*/
// Created: APE 2004-06-02
// -----------------------------------------------------------------------------
void MOS_Options::WriteSettings( MT_Settings& settings )
{
    settings.beginGroup( "/Options" );
    settings.writeEntry( "/smallText",      nDrawSmallText_ );
    settings.writeEntry( "/largeText",      nDrawLargeText_ );
    settings.writeEntry( "/details",        nDrawDetails_ );
    settings.writeEntry( "/tacticalLines",  nDrawTacticalLines_ );
    settings.writeEntry( "/visionLines",    nDrawVisionLines_ );
    settings.writeEntry( "/visionCones",    nDrawVisionCones_ );
    settings.writeEntry( "/visionSurfaces", nDrawVisionSurfaces_ );
    settings.writeEntry( "/paths",          nDrawPaths_ );
    settings.writeEntry( "/oldpaths",       bDrawOldPaths_ );
    settings.writeEntry( "/displayWeather", bDisplayWeather_ );
    settings.writeEntry( "/openToItem",     bOpenTreeToItem_ );
    settings.writeEntry( "/gridStep",       rGridStep_ );
    settings.writeEntry( "/fontSize",       nFontSize_ );
    settings.writeEntry( "/autoSaveLines",  bSaveLoadTacticalLines_ );
    settings.endGroup();
}


// -----------------------------------------------------------------------------
// Name: MOS_Options::ReadSettings
/** @param  settings 
*/
// Created: APE 2004-06-02
// -----------------------------------------------------------------------------
void MOS_Options::ReadSettings( MT_Settings& settings )
{
    settings.beginGroup( "/Options" );
    nDrawSmallText_   = (E_State)settings.readNumEntry( "/smallText", eAuto );
    nDrawLargeText_   = (E_State)settings.readNumEntry( "/largeText", eAuto );
    nDrawDetails_     = (E_State)settings.readNumEntry( "/details",   eAuto );
    nDrawTacticalLines_=(E_State)settings.readNumEntry( "/tacticalLines", eAuto );
    nDrawVisionLines_ = (E_State)settings.readNumEntry( "/visionLines", eAuto );
    nDrawVisionCones_ = (E_State)settings.readNumEntry( "/visionCones", eOff );
    nDrawVisionSurfaces_ = (E_State)settings.readNumEntry( "/visionSurfaces", eOff );
    nDrawPaths_       = (E_State)settings.readNumEntry( "/paths", eOn );
    bDrawOldPaths_    = settings.readBoolEntry( "/oldpaths", false );
    bDisplayWeather_  = settings.readBoolEntry( "/displayWeather", false );
    bOpenTreeToItem_  = settings.readBoolEntry( "/openToItem",  true );
    rGridStep_        = settings.readNumEntry( "/gridStep", 5.0f );
    nFontSize_        = settings.readNumEntry( "/fontSize", 10 );
    bSaveLoadTacticalLines_ = settings.readBoolEntry( "/autoSaveLines", true );
    settings.endGroup();
}


