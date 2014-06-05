// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#ifndef __ScoreModel_h_
#define __ScoreModel_h_

#include "protocol/AarSenders.h"
#include <tools/ElementObserver_ABC.h>
#include <tools/Resolver.h>

namespace kernel
{
    class Controllers;
}

class IndicatorRequest;
class IndicatorRequests;
class Publisher_ABC;
class Score;
class ScoreDefinitions;
class Services;

// =============================================================================
/** @class  ScoreModel
    @brief  ScoreModel
*/
// Created: SBO 2009-03-12
// =============================================================================
class ScoreModel : public tools::Resolver< Score, QString >
                 , public tools::Observer_ABC
                 , public tools::ElementObserver_ABC< Services >
{
public:
    //! @name Constructors/Destructor
    //@{
             ScoreModel( kernel::Controllers& controllers, Publisher_ABC& publisher, const ScoreDefinitions& definitions );
    virtual ~ScoreModel();
    //@}

    //! @name Operations
    //@{
    void Purge();
    void Update( const sword::Indicator& message );
    void Update( const sword::PlotResult& message );
    IndicatorRequest& CreateRequest( Score& score );
    //@}

private:
    //! @name Copy/Assignment
    //@{
    ScoreModel( const ScoreModel& );            //!< Copy constructor
    ScoreModel& operator=( const ScoreModel& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    virtual void NotifyUpdated( const Services& services );
    //@}

    //! @name Types
    //@{
    typedef std::map< const Score*, IndicatorRequest* > T_ScoreRequests;
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controllers& controllers_;
    Publisher_ABC& publisher_;
    const ScoreDefinitions& definitions_;
    std::unique_ptr< IndicatorRequests > requests_;
    T_ScoreRequests scoreRequests_;
    bool hasReplay_;
    //@}
};

#endif // __ScoreModel_h_
