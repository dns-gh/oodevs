// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA GROUP
//
// *****************************************************************************

#ifndef __edxl_Publisher_h_
#define __edxl_Publisher_h_

#include "Publisher_ABC.h"

#ifdef _MSC_VER
#pragma warning( push, 0 )
#endif
#include <boost/program_options.hpp>
#include <boost/thread/recursive_mutex.hpp>
#ifdef _MSC_VER
#pragma warning( pop )
#endif

namespace xml
{
    class xistream;
}

namespace edxl
{
    class EDXLReport;

// =============================================================================
/** @class  Publisher
    @brief  Publisher
*/
// Created: SBO 2008-04-02
// =============================================================================
class Publisher : public Publisher_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
    explicit Publisher( const boost::program_options::variables_map& options );
    virtual ~Publisher();
    //@}

    //! @name Operations
    //@{
    virtual void PullSituation( const std::string& message, ResponseHandler_ABC& response );
    virtual void PushReport( const std::string& message );
    //@}

private:
    //! @name Copy/Assignment
    //@{
    Publisher( const Publisher& );            //!< Copy constructor
    Publisher& operator=( const Publisher& ); //!< Assignment operator
    //@}}

private:
    //! @name Member data
    //@{
    boost::recursive_mutex mutex_;
    std::string host_;
    std::string postURI_;
    std::string getURI_;
    std::string frequency_;
    bool useSsl_;
    bool log_;
    //@}
};

}

#endif // __edxl_Publisher_h_
