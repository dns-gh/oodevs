// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#ifndef __Simulation_h_
#define __Simulation_h_

#include "clients_kernel/Time_ABC.h"
#include "Profiling.h"
#include <map>

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
    class Timeskip;
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
    struct sBeginSkip {};
    struct sEndSkip {};
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
    void Update( const sword::Timeskip& message );
    void Update( const sword::BeginSkip& message );
    void Update( const sword::EndSkip& message );
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

    //! @name Simulation_ABC methods
    //@{
    virtual QDateTime GetDateTime() const;
    virtual QDateTime GetInitialDateTime() const;
    virtual unsigned int GetTickDuration() const;
    virtual QString GetTimeAsString() const;
    //@}

    //! @name Accessors
    //@{
    QDateTime GetTime( uint32_t tick ) const;
    unsigned int GetFirstTick() const;           //!< first recorded tick (replayer)
    unsigned int GetCurrentTick() const;         //!< tick since simulation start
    unsigned int GetTickCount() const;           //!< total tick count (replayer)
    QDateTime GetEndDateTime() const;
    QString GetDateAsString() const;
    QDateTime GetRealDateTime() const;
    QString GetRealTimeAsString() const;
    QString GetRealDateAsString() const;

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
    std::string initialDate_;
    std::string endDateTime_;
    std::string simDate_;
    std::string realDate_;
    bool paused_;
    bool connected_;
    bool initialized_;
    sCheckPoint checkPoint_;
    std::string simulationHost_;
    Profiling profiling_;
    std::map< uint32_t, uint32_t > skips_;
    //@}
};

#endif // __Simulation_h_
