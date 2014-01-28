// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __LogisticsConsign_ABC_h_
#define __LogisticsConsign_ABC_h_

#include "clients_gui/Drawable_ABC.h"
#include <boost/noncopyable.hpp>

namespace kernel
{
    class Agent_ABC;
    class Controller;
    class Entity_ABC;
}

namespace sword
{
    class LogHistoryEntry;
}

namespace google
{
    namespace protobuf
    {
        template< typename T > class RepeatedPtrField;
    }
}

class Simulation;

// =============================================================================
/** @class  LogisticsConsign_ABC
    @brief  LogisticsConsign_ABC
*/
// Created: MMC 2013-09-16
// =============================================================================
class LogisticsConsign_ABC : public gui::Drawable_ABC
                           , private boost::noncopyable
{
public:
    //! @name Constructor / Destructor
    //@{
             LogisticsConsign_ABC( unsigned int nID,  kernel::Controller& controller, const Simulation& simulation, unsigned int creationTick );
    virtual ~LogisticsConsign_ABC();
    //@}

    //! @name Accessors
    //@{
    unsigned int GetId() const;
    QString GetCreationTime() const;
    QString GetCurrentEndTime() const;
    virtual bool NeedResolution() const;
    virtual bool RefersToAgent( unsigned long id ) const = 0;
    virtual bool RefersToAgent( const std::set< unsigned long >& id ) const = 0;
    virtual QString GetCurrentStartedTime() const = 0;
    virtual QString GetStatusDisplay() const = 0;
    virtual kernel::Agent_ABC* GetConsumer() const = 0;
    virtual kernel::Entity_ABC* GetHandler() const = 0;
    //@}

    //! @name Operations
    //@{
    void UpdateHistory( int start, int end,
        const google::protobuf::RepeatedPtrField< sword::LogHistoryEntry >& history,
        unsigned int currentTick );
    //@}

    //! @name Types
    //@{
    struct HistoryState
    {
        HistoryState()
             : nStatus_( 0 )
             , handler_( 0 )
             , startedTick_( 0 )
             , endedTick_( 0 )
        {}

        int nStatus_;
        kernel::Entity_ABC* handler_;
        unsigned int startedTick_;
        unsigned int endedTick_;
    };

    class History
    {
    public:
        History( const LogisticsConsign_ABC& consign ) 
            : consign_( &consign ) {}

        void Clear()
        {
            states_.clear();
        }

        void Add( const HistoryState& state )
        {
            if( !states_.empty() )
                states_.back().endedTick_ = state.startedTick_;
            states_.push_back( state );
        }

        const LogisticsConsign_ABC& GetConsign() const
        { 
            return *consign_;
        }

        const std::vector< HistoryState >& GetStates() const
        {
            return states_;
        }

    private:
        const LogisticsConsign_ABC* consign_;
        std::vector< HistoryState > states_;
    };
    //@}

    //! @name Operations
    //@{
    const History& GetHistory() const;
    QString GetStatusLastStarted( int status ) const;
    //@}

private:
    virtual kernel::Entity_ABC* GetRequestHandler( uint32_t entityId ) const = 0;

public:
    //! @name Helpers
    //@{
    virtual QString GetStatusDisplay( int status ) const = 0;
    QString ConvertTickToTimeString( unsigned int tick ) const;
    //@}

protected:
    //! @name Member data
    //@{
    kernel::Controller& controller_;
    const Simulation& simulation_;
    const unsigned int nID_;
    unsigned int currentStateEndTick_;
    unsigned int creationTick_;
    std::unique_ptr< History > history_;
    //@}
};

#endif // __LogisticsConsign_ABC_h_
