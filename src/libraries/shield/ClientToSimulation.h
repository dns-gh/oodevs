// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#ifndef shield_ClientToSimulation_h_
#define shield_ClientToSimulation_h_

#include "protocol/Protocol.h"
#pragma warning( push, 0 )
#include "proto/ClientToSim.pb.h"
#pragma warning( pop )

namespace shield
{
// =============================================================================
/** @class  ClientToSimulation
    @brief  Client to simulation converter
*/
// Created: MCO 2010-11-25
// =============================================================================
class ClientToSimulation
{
public:
    //! @name Operations
    //@{
    static void Convert( const Common::MsgControlChangeTimeFactor& from, sword::ControlChangeTimeFactor* to );
    static void Convert( const Common::MsgControlStop& from, sword::ControlStop* to );
    static void Convert( const Common::MsgControlPause& from, sword::ControlPause* to );
    static void Convert( const Common::MsgControlResume& from, sword::ControlResume* to );
    static void Convert( const MsgsClientToSim::MsgControlDatetimeChange& from, sword::ControlDateTimeChange* to );
    static void Convert( const MsgsClientToSim::MsgControlCheckPointSaveNow& from, sword::ControlCheckPointSaveNow* to );
    static void Convert( const MsgsClientToSim::MsgControlCheckPointSetFrequency& from, sword::ControlCheckPointSetFrequency* to );
    static void Convert( const MsgsClientToSim::MsgControlEnableVisionCones& from, sword::ControlEnableVisionCones* to );
    static void Convert( const Common::MsgUnitOrder& from, sword::UnitOrder* to );
    static void Convert( const Common::MsgAutomatOrder& from, sword::AutomatOrder* to );
    static void Convert( const Common::MsgCrowdOrder& from, sword::CrowdOrder* to );
    static void Convert( const MsgsClientToSim::MsgFragOrder& from, sword::FragOrder* to );
    static void Convert( const MsgsClientToSim::MsgSetAutomatMode& from, sword::SetAutomatMode* to );
    static void Convert( const MsgsClientToSim::MsgUnitMagicAction& from, sword::UnitMagicAction* to );
    static void Convert( const MsgsClientToSim::MsgObjectMagicAction& from, sword::ObjectMagicAction* to );
    static void Convert( const MsgsClientToSim::MsgKnowledgeMagicAction& from, sword::KnowledgeMagicAction* to );
    static void Convert( const MsgsClientToSim::MsgMagicAction& from, sword::MagicAction* to );
    static void Convert( const MsgsClientToSim::MsgControlCheckPointListRequest& from, sword::ControlCheckPointListRequest* to );
    static void Convert( const MsgsClientToSim::MsgControlCheckPointDeleteRequest& from, sword::ControlCheckPointDeleteRequest* to );
    static void Convert( const MsgsClientToSim::MsgOrderStream& from, sword::OrderStream* to );

    static void ConvertUnitMagicActionChangeLogisticLinks  ( const Common::MsgMissionParameters& from, sword::MissionParameters& to );
    static void ConvertUnitMagicActionLogSupplyChangeQuotas( const Common::MsgMissionParameters& from, sword::MissionParameters& to );
    //@}
};

}

#endif // shield_ClientToSimulation_h_
