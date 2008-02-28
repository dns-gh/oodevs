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

#include "game_asn/Simulation.h"

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
    ~SimulationModel();
    //@}

    //! @name Operations
    //@{
    void Update        ( const ASN1T_MsgControlInformation&			 msg );
    void Update        ( const ASN1T_MsgControlBeginTick&			 msg );
    void Update        ( const ASN1T_MsgControlEndTick&			     msg );
	void Update_Stop   ( const ASN1T_MsgControlStopAck&			     msg ); //$$$
	void Update_Pause  ( const ASN1T_MsgControlPauseAck&			 msg ); //$$$
	void Update_Resume ( const ASN1T_MsgControlResumeAck&			 msg ); //$$$
	void Update		   ( const ASN1T_MsgControlChangeTimeFactorAck& msg );
//    void Update        ( const ASN1T_MsgControlCheckPointSetFrequencyAck& msg );
    void Send          ( ClientPublisher_ABC& publisher ) const;
    void SendReplayInfo( ClientPublisher_ABC& publisher, unsigned totalTicks, ASN1T_EnumSimulationState status, unsigned factor ) const;
    void Reset         ();

    void SendFirstTick( ClientPublisher_ABC& publisher ) const;
    //@}

private:
   unsigned int      nCurrentTick_;
   std::string       date_;
   unsigned int      nTickDuration_;
   unsigned int      nTimeFactor_;
   unsigned int      nCheckpointFrequency_;
   ASN1T_EnumSimulationState nSimState_;

   //$$$ BULLSHIT
   bool         bSendVisionCones_;
   bool         bProfilingEnabled_;
};

}

#endif // __SimulationModel_h_
