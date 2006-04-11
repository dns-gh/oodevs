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
// $Archive: /MVW_v10/Build/SDK/Light2/src/Options.cpp $
// $Author: Age $
// $Modtime: 16/05/05 16:59 $
// $Revision: 10 $
// $Workfile: Options.cpp $
//
// *****************************************************************************

#ifdef __GNUG__
#   pragma implementation
#endif

#include "astec_pch.h"
#include "Options.h"

#include "MT_Settings.h"
#include "MT/MT_IO/MT_CommandLine.h"

// -----------------------------------------------------------------------------
// Name: Options constructor
// Created: APE 2004-03-16
// -----------------------------------------------------------------------------
Options::Options()
    : nDrawSmallText_           ( eAuto )
    , nDrawLargeText_           ( eAuto )
    , nDrawDetails_             ( eAuto )
    , nDrawTacticalLines_       ( eOn )
    , nDrawVisionLines_         ( eAuto )
    , nDrawVisionCones_         ( eOff )
    , nDrawVisionSurfaces_      ( eOff )
    , nDrawPaths_               ( eOn )
    , bDrawOldPaths_            ( false )
    , bDisplayWeather_          ( false )
    , nDisplayLogLinks_         ( eOff )
    , nDisplayMissingLogLinks_  ( eOff )
    , rGridStep_                ( 1.0 )
    , bOpenTreeToItem_          ( true )
    , nPlayedTeam_              ( eController )
    , bControllerMode_          ( true )
    , nFontSize_                ( 10 )
    , bSaveLoadTacticalLines_   ( true )
    , b3D_                      ( false )
    , bNoList_                  ( true )
    , bDisplayRealTimeLog_      ( false )
    , bDrawObjetIcons_          ( true )
    , bDisplayHoveredInfo_      ( true )
    , bDisplayRConMap_          ( true )
    , bDisplayMessagesOnMap_    ( false )
    , bDisplayTracesOnMap_      ( false )
    , bDisplayIdentificationLevelOnMap_ ( false )
    , bDisplayOnlySubscribedAgentsRC_   ( false )
    , bAskForTacticalLineSavingOnExit_  ( true )
{
    MT_CommandLine arguments( qApp->argc(), qApp->argv() );
    bNoList_ = arguments.IsOptionSet( "-nolist" );
}


// -----------------------------------------------------------------------------
// Name: Options destructor
// Created: APE 2004-03-16
// -----------------------------------------------------------------------------
Options::~Options()
{
}

// -----------------------------------------------------------------------------
// Name: Options::WriteOptions
// Created: APE 2004-07-12
// -----------------------------------------------------------------------------
void Options::Write() const
{
    MT_Settings settings;
    settings.setPath( "MASA", "Light2" );
    WriteSettings( settings );
}
    

// -----------------------------------------------------------------------------
// Name: Options::ReadOptions
// Created: APE 2004-07-12
// -----------------------------------------------------------------------------
void Options::Read()
{
    MT_Settings settings;
    settings.setPath( "MASA", "Light2" );
    ReadSettings( settings );
}


// -----------------------------------------------------------------------------
// Name: Options::WriteSettings
// Created: APE 2004-06-02
// -----------------------------------------------------------------------------
void Options::WriteSettings( MT_Settings& settings ) const
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
    settings.writeEntry( "/displayHoveredInfo",                 bDisplayHoveredInfo_ );
    settings.writeEntry( "/displayIdentificationLevelOnMap",    bDisplayIdentificationLevelOnMap_ );
    settings.writeEntry( "/displayMessagesOnMap",               bDisplayMessagesOnMap_ );
    settings.writeEntry( "/displayRCOnMap",                     bDisplayRConMap_ );
    settings.writeEntry( "/displayRealTimeLog",                 bDisplayRealTimeLog_ );
    settings.writeEntry( "/dislayTracesOnMap",                  bDisplayTracesOnMap_ );
    settings.writeEntry( "/displayLogLinks",                    nDisplayLogLinks_ );
    settings.writeEntry( "/displayMissingLogLinks",             nDisplayMissingLogLinks_ );
    settings.writeEntry( "/DisplayOnlySubscribedAgentsRC_",     bDisplayOnlySubscribedAgentsRC_ );
    settings.writeEntry( "/AskForTacticalLineSavingOnExit",     bAskForTacticalLineSavingOnExit_ );
    settings.endGroup();
}


// -----------------------------------------------------------------------------
// Name: Options::ReadSettings
/** @param  settings 
*/
// Created: APE 2004-06-02
// -----------------------------------------------------------------------------
void Options::ReadSettings( MT_Settings& settings )
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
    rGridStep_                          = settings.readNumEntry( "/gridStep", 1.0f );
    nFontSize_        = settings.readNumEntry( "/fontSize", 10 );
    bSaveLoadTacticalLines_ = settings.readBoolEntry( "/autoSaveLines", true );
    bDisplayHoveredInfo_                = settings.readBoolEntry( "/displayHoveredInfo", true );
    bDisplayIdentificationLevelOnMap_   = settings.readBoolEntry( "/displayIdentificationLevelOnMap", false );
    bDisplayMessagesOnMap_              = settings.readBoolEntry( "/displayMessagesOnMap", false );
    bDisplayRConMap_                    = settings.readBoolEntry( "/displayRCOnMap", true );
    bDisplayRealTimeLog_                = settings.readBoolEntry( "/displayRealTimeLog", false );
    nDisplayLogLinks_                   = (E_State)settings.readNumEntry( "/displayLogLinks", eNone );
    nDisplayMissingLogLinks_            = (E_State)settings.readNumEntry( "/displayMissingLogLinks", eNone );
    bDisplayOnlySubscribedAgentsRC_     = settings.readBoolEntry( "/DisplayOnlySubscribedAgentsRC_", false );
    bAskForTacticalLineSavingOnExit_    = settings.readBoolEntry( "/AskForTacticalLineSavingOnExit", true );
    
    settings.endGroup();
}


