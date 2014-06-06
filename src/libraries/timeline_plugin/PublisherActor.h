// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __plugins_timeline_PublisherActor_h_
#define __plugins_timeline_PublisherActor_h_

#include "Publisher_ABC.h"
#pragma warning( push, 0 )
#include <boost/thread/recursive_mutex.hpp>
#pragma pop

namespace tools
{
namespace thread
{
    class ThreadPool;
}
}

namespace plugins
{
namespace timeline
{

// =============================================================================
/** @class  PublisherActor
    @brief  PublisherActor
*/
// Created: AGE 2008-05-30
// =============================================================================
class PublisherActor : public Publisher_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
    explicit PublisherActor( std::unique_ptr< Publisher_ABC > base );
    virtual ~PublisherActor();
    //@}

    //! @name Operations
    //@{
    virtual void PullSituation( const std::string& message, const std::string& timestamp, ResponseHandler_ABC& response );
    virtual void PushReport( const std::string& message );
    virtual void RestartScenario();
    //@}

private:
    //! @name Copy/Assignment
    //@{
    PublisherActor( const PublisherActor& );            //!< Copy constructor
    PublisherActor& operator=( const PublisherActor& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    void DoPushReport( const std::string& message );
    void DoPullSituation( const std::string& message, const std::string& timestamp, ResponseHandler_ABC& handler );
    void DoRestartScenario();
    //@}

    //! @name Types
    //@{
    typedef std::pair< ResponseHandler_ABC*, std::string > T_Answer;
    typedef std::vector< T_Answer >                        T_Answers;
    //@}

private:
    //! @name Member data
    //@{
    std::unique_ptr< Publisher_ABC > base_;
    std::unique_ptr< tools::thread::ThreadPool > thread_;
    boost::recursive_mutex mutex_;
    T_Answers answers_;
    //@}
};

}
}

#endif // __plugins_timeline_PublisherActor_h_
