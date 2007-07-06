// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __Simulation_h_
#define __Simulation_h_

#include "game_asn/Asn.h"
#include "Profiling.h"

namespace kernel
{
    class Controller;
}

// =============================================================================
/** @class  Simulation
    @brief  Simulation
*/
// Created: AGE 2006-02-10
// =============================================================================
class Simulation
{

public:
    struct sStartTick{};
    struct sEndTick{};
    struct sCheckPoint { bool start_; };
    struct sConnection { bool connected_; };
    
public:
    //! @name Constructors/Destructor
    //@{
    explicit Simulation( kernel::Controller& controller );
    virtual ~Simulation();
    //@}

    //! @name Operations
    //@{
    void Connect( const std::string& host );
    void Disconnect();
    void Pause( bool );
    void ChangeSpeed( int timeFactor );
    void Update( const ASN1T_MsgControlInformation& asnMsg );
    void Update( const ASN1T_MsgControlReplayInformation& asnMsg );
    void Update( const ASN1T_MsgControlProfilingInformation& message );
    void BeginTick( int tick );
    void EndTick( const ASN1T_MsgControlEndTick& asnMsg );

    void BeginCheckPoint();
    void EndCheckPoint();
    //@}

    //! @name Accessors
    //@{
    const std::string& GetSimulationHost() const;
    int  GetTime() const;                    //!< seconds since simulation start
    unsigned GetCurrentTick() const;         //!< tick since simulation start
    unsigned GetTickCount() const;           //!< total tick count (replayer)
    int GetDay() const; 
    QString GetTimeAsString() const; 
    bool IsPaused() const;
    bool IsConnected() const;
    int  GetSpeed() const;
    float GetEffectiveSpeed() const;         //!< average speed on last ten updates
    unsigned long GetMemory() const;                //!< last memory usage
    unsigned long GetVirtualMemory() const;         //!< last virtual memory usage
    unsigned long GetShortPathfinds() const;        //!< last short pathfinds count
    unsigned long GetLongPathfinds() const;         //!< last long pathfinds count
    //@}
    
private:
    //! @name Copy/Assignement
    //@{
    Simulation( const Simulation& );            //!< Copy constructor
    Simulation& operator=( const Simulation& ); //!< Assignement operator
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controller& controller_;
    unsigned int tickDuration_;
    unsigned int timeFactor_;
    unsigned int tickCount_;
    int time_;
    bool paused_;
    bool connected_;
    sConnection connection_;
    sStartTick startTick_;
    sEndTick endTick_;
    sCheckPoint checkPoint_;
    std::string simulationHost_;
    Profiling profiling_;
    //@}
};

#endif // __Simulation_h_
