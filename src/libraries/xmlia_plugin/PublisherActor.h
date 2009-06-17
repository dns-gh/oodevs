// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __PublisherActor_h_
#define __PublisherActor_h_

#include "Publisher_ABC.h"
#include <boost/thread/recursive_mutex.hpp>

namespace tools
{
namespace thread
{
    class ThreadPool;
}
}

namespace plugins
{
namespace xmlia
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
    explicit PublisherActor( std::auto_ptr< Publisher_ABC > base );
    virtual ~PublisherActor();
    //@}

    //! @name Operations
    //@{
    virtual void PushReports();
    virtual xml::xostream& CreateReport();
    virtual void PullOrder( const std::string& message, ResponseHandler_ABC& handler );
    void CommitOrders();
    virtual std::string GetUrlReports() { return "";};
    virtual std::string GetXmliaMessage( const std::string& url ){ return "";};
    //virtual std::string GetUrlId() { return ""; };
    //virtual std::string PushReport( const std::string& xmliaMessage, const std::string& urlId ){ return ""; };
    //@}

private:
    //! @name Copy/Assignment
    //@{
    PublisherActor( const PublisherActor& );            //!< Copy constructor
    PublisherActor& operator=( const PublisherActor& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    void DoPushReports();
    void DoPullOrder( const std::string& message, ResponseHandler_ABC& handler );
    //@}

    //! @name Types
    //@{
    typedef std::pair< ResponseHandler_ABC*, std::string > T_Answer;
    typedef std::vector< T_Answer >                        T_Answers;
    //@}

private:
    //! @name Member data
    //@{
    std::auto_ptr< Publisher_ABC > base_;
    std::auto_ptr< tools::thread::ThreadPool > thread_;
    boost::recursive_mutex mutex_;
    T_Answers answers_;
    //@}
};

}
}

#endif // __PublisherActor_h_
