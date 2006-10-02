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

#include "AsnTypes.h"

namespace dispatcher
{
class Publisher_ABC;

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
    void Update        ( const ASN1T_MsgCtrlInfo&      msg );
    void Update        ( const ASN1T_MsgCtrlBeginTick& msg );
    void Update        ( const ASN1T_MsgCtrlEndTick&   msg );
//    void Update        ( const ASN1T_MsgCtrlCheckPointSetFrequencyAck& msg );
    void Update_MsgInit( DIN::DIN_Input& msg ); //$$$ TMP DIN
    void Send          ( Publisher_ABC& publisher ) const;
    //@}

private:
   unsigned int      nCurrentTick_;
   unsigned int      nTickDuration_;
   unsigned int      nTimeFactor_;
   unsigned int      nExerciseID_;
   unsigned int      nCheckpointFrequency_;
   ASN1T_EnumEtatSim nSimState_;

   //$$$ BULLSHIT
   bool         bSendVisionCones_;
   bool         bProfilingEnabled_;
   std::string  strConfigPath_;
};

}

#endif // __SimulationModel_h_
