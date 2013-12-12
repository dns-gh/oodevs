// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2013 MASA Group
//
// *****************************************************************************

#ifndef EventOrderViewState_h
#define EventOrderViewState_h

#include "ENT/ENT_Tr.h"
#include <string>
#include <vector>

namespace gui
{

// =============================================================================
/** @class  EventOrderViewState
    @brief  EventOrderViewState
*/
// Created: ABR 2013-11-18
// =============================================================================
struct EventOrderViewState
{
    EventOrderViewState()
        : currentType_( eMissionType_Pawn )
        , invalid_( false )
        , missionSelector_( false )
    {
        // NOTHING
    }

    EventOrderViewState( const std::vector< E_MissionType >& types,
                         E_MissionType currentType,
                         const std::vector< std::string >& missions,
                         const std::string& currentMission,
                         const std::vector< std::string >& disabledMissions,
                         bool invalid,
                         bool missionSelector )
        : types_( types )
        , currentType_( currentType )
        , missions_( missions )
        , currentMission_( currentMission )
        , disabledMissions_( disabledMissions )
        , invalid_( invalid )
        , missionSelector_( missionSelector )
    {
        // NOTHING
    }

    void Purge()
    {
        types_.clear();
        currentType_ = eMissionType_Pawn;
        missions_.clear();
        currentMission_.clear();
        disabledMissions_.clear();
        invalid_ = false;
        missionSelector_ = false;
    }

    std::vector< E_MissionType > types_;
    E_MissionType currentType_;
    std::vector< std::string > missions_;
    std::string currentMission_;
    std::vector< std::string > disabledMissions_;
    bool invalid_;
    bool missionSelector_;
};

} //! namespace gui

#endif // EventOrderViewState_h
