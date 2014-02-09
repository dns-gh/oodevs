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

#include "clients_kernel/Time_ABC.h"
#include "Profiling.h"

namespace kernel
{
    class Controller;
}

namespace sword
{
    class ControlInformation;
    class ControlReplayInformation;
    class ControlProfilingInformation;
    class ControlBeginTick;
    class ControlEndTick;
    class ControlSendCurrentStateEnd;
    class ControlCheckPointSaveEnd;
    class NewDataChunkNotification;
    class TimeTable;
}

// =============================================================================
/** @class  Simulation
    @brief  Simulation
*/
// Created: AGE 2006-02-10
// =============================================================================
class Simulation : public kernel::Time_ABC
{
public:
    struct sStartTick {};
    struct sEndTick {};
    struct sCheckPoint
    {
        bool start_;
        tools::Path name_;
    };
    struct sTimeTable : private boost::noncopyable
    {
        explicit sTimeTable( const sword::TimeTable& timeTable ) : timeTable_( timeTable ) {}
        const sword::TimeTable& timeTable_;
    };
    struct Reconnection
    {
        explicit Reconnection( const std::string& login ) : login_( login ) {}
                 Reconnection( const std::string& login, const std::string& password )
                     : login_( login ), password_( password ) {}
        std::string login_;
        std::string password_;
    };

public:
    //! @name Constructors/Destructor
    //@{
    explicit Simulation( kernel::Controller& controller );
    virtual ~Simulation();
    //@}

    //! @name Updates
    //@{
    void Update( const sword::ControlInformation& message );
    void Update( const sword::ControlReplayInformation& message );
    void Update( const sword::ControlProfilingInformation& message );
    void Update( const sword::ControlBeginTick& message );
    void Update( const sword::ControlEndTick& message );
    void Update( const sword::ControlSendCurrentStateEnd& message );
    void Update( const sword::NewDataChunkNotification& message );
    void Update( const sword::TimeTable& message );
    //@}

    //! @name Operations
    //@{
    void Connect( const std::string& host );
    void Disconnect();
    void Pause( bool paused );
    void ChangeSpeed( int timeFactor );

    void BeginCheckPoint();
    void EndCheckPoint( const sword::ControlCheckPointSaveEnd& message );
    //@}

    //! @name Accessors
    //@{
    virtual QDateTime GetInitialDateTime() const;
    virtual QDateTime GetDateTime() const;
    virtual QString GetTimeAsString() const;
    QDateTime GetEndDateTime() const;
    QString GetDateAsString() const;
    QDateTime GetRealDateTime() const;
    QString GetRealTimeAsString() const;
    QString GetRealDateAsString() const;

    int GetTime() const;                    //!< seconds since simulation start

    unsigned int GetCurrentTick() const;         //!< tick since simulation start
    unsigned int GetTickCount() const;           //!< total tick count (replayer)
    unsigned int GetFirstTick() const;           //!< first recorded tick (replayer)
    virtual unsigned int GetTickDuration() const;

    bool IsPaused() const;
    bool IsConnected() const;
    bool IsInitialized() const;
    const std::string& GetSimulationHost() const;

    int GetSpeed() const;
    float GetEffectiveSpeed() const;         //!< average speed on last ten updates
    float GetActualSpeed() const;            //!< tick duration based on last 2 end-ticks

    unsigned long GetMemory() const;                //!< last memory usage
    unsigned long GetVirtualMemory() const;         //!< last virtual memory usage
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controller& controller_;
    unsigned int tickDuration_;
    unsigned int timeFactor_;
    unsigned int currentTick_;
    unsigned int tickCount_;
    unsigned int firstTick_;
    int time_;
    std::string initialDate_;
    std::string endDateTime_;
    std::string simDate_;
    std::string realDate_;
    bool paused_;
    bool connected_;
    bool initialized_;
    sStartTick startTick_;
    sEndTick endTick_;
    sCheckPoint checkPoint_;
    std::string simulationHost_;
    Profiling profiling_;
    //@}
};

#endif // __Simulation_h_
