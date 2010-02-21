 // *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __SimulationModel_h_
#define __SimulationModel_h_



namespace Common
{
    enum EnumSimulationState;
}

namespace MsgsSimToClient
{
    class MsgControlStopAck;
    class MsgControlPauseAck;
    class MsgControlResumeAck;
    class MsgControlChangeTimeFactorAck;

    class MsgControlInformation;
    class MsgControlBeginTick;
    class MsgControlEndTick;
}

namespace dispatcher
{
    class ClientPublisher_ABC;

// =============================================================================
/** @class  SimulationModel
    @brief  SimulationModel
*/
// Created: NLD 2006-09-19
// =============================================================================
class SimulationModel
{
public:
    //! @name Constructors/Destructor
    //@{
     SimulationModel();
    virtual ~SimulationModel();
    //@}

    //! @name Operations
    //@{
    void Update       ( const MsgsSimToClient::MsgControlInformation&         msg );
    void Update       ( const MsgsSimToClient::MsgControlBeginTick&           msg );
    void Update       ( const MsgsSimToClient::MsgControlEndTick&             msg );
    void Update_Stop  ( const MsgsSimToClient::MsgControlStopAck&             msg ); //$$$
    void Update_Pause ( const MsgsSimToClient::MsgControlPauseAck&            msg ); //$$$
    void Update_Resume( const MsgsSimToClient::MsgControlResumeAck&           msg ); //$$$
    void Update       ( const MsgsSimToClient::MsgControlChangeTimeFactorAck& msg );
//    void Update        ( const MsgControlCheckPointSetFrequencyAck& msg );
    void Send          ( ClientPublisher_ABC& publisher ) const;
    void SendReplayInfo( ClientPublisher_ABC& publisher, unsigned totalTicks, Common::EnumSimulationState status, unsigned factor ) const;
    void Reset         ();

    void SendFirstTick( ClientPublisher_ABC& publisher ) const;
    //@}

private:
   unsigned int      nCurrentTick_;
   std::string       initialDate_;
   std::string       date_;
   unsigned int      nTickDuration_;
   unsigned int      nTimeFactor_;
   unsigned int      nCheckpointFrequency_;
   Common::EnumSimulationState nSimState_;

   //$$$ BULLSHIT
   bool         bSendVisionCones_;
   bool         bProfilingEnabled_;
};

}

#endif // __SimulationModel_h_
