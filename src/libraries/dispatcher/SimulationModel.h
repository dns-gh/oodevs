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

namespace sword
{
    enum EnumSimulationState;
}

namespace sword
{
    class ControlStopAck;
    class ControlPauseAck;
    class ControlResumeAck;
    class ControlChangeTimeFactorAck;

    class ControlInformation;
    class ControlBeginTick;
    class ControlEndTick;
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
    void Update       ( const sword::ControlInformation&         msg );
    void Update       ( const sword::ControlBeginTick&           msg );
    void Update       ( const sword::ControlEndTick&             msg );
    void Update_Stop  ( const sword::ControlStopAck&             msg ); //$$$
    void Update_Pause ( const sword::ControlPauseAck&            msg ); //$$$
    void Update_Resume( const sword::ControlResumeAck&           msg ); //$$$
    void Update       ( const sword::ControlChangeTimeFactorAck& msg );
//    void Update        ( const ControlCheckPointSetFrequencyAck& msg );
    void Send          ( ClientPublisher_ABC& publisher ) const;
    void SendReplayInfo( ClientPublisher_ABC& publisher, unsigned totalTicks, sword::EnumSimulationState status, unsigned factor ) const;
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
   sword::EnumSimulationState nSimState_;

   //$$$ BULLSHIT
   bool         bSendVisionCones_;
   bool         bProfilingEnabled_;
};

}

#endif // __SimulationModel_h_
