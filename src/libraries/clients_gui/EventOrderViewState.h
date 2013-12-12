// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2013 MASA Group
//
// *****************************************************************************

#ifndef __EventOrderViewState_h_
#define __EventOrderViewState_h_

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
    //! @name Constructors
    //@{
    EventOrderViewState()
        : target_( 0 )
        , currentType_( eMissionType_Pawn )
        , invalid_( false )
        , missionSelector_( false )
    {
        // NOTHING
    }
    EventOrderViewState( unsigned long target,
                         const std::vector< E_MissionType >& types,
                         E_MissionType currentType,
                         const std::vector< std::string >& missions,
                         const std::string& currentMission,
                         const std::vector< std::string >& disabledMissions,
                         bool invalid,
                         bool missionSelector )
        : target_( target )
        , types_( types )
        , currentType_( currentType )
        , missions_( missions )
        , currentMission_( currentMission )
        , disabledMissions_( disabledMissions )
        , invalid_( invalid )
        , missionSelector_( missionSelector )
    {
        // NOTHING
    }
    //@}

    //! @name Operations
    //@{
    void Purge()
    {
        *this = EventOrderViewState();
    }
    //@}

    //! @name Member data
    //@{
    unsigned long target_;
    std::vector< E_MissionType > types_;
    E_MissionType currentType_;
    std::vector< std::string > missions_;
    std::string currentMission_;
    std::vector< std::string > disabledMissions_;
    bool invalid_;
    bool missionSelector_;
    //@}
};

} //! namespace gui

#endif // __EventOrderViewState_h_
