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

#include "clients_kernel/ElementObserver_ABC.h"
#include "clients_kernel/Resolver.h"
#include "game_asn/Aar.h"

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
class ScoreModel : public kernel::Resolver< Score, QString >
                 , public kernel::Observer_ABC
                 , public kernel::ElementObserver_ABC< Services >
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
    void Update( const ASN1T_MsgIndicator& message );
    void Update( const ASN1T_MsgPlotResult& message );
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
    std::auto_ptr< IndicatorRequests > requests_;
    T_ScoreRequests scoreRequests_;
    bool hasReplay_;
    //@}
};

#endif // __ScoreModel_h_
