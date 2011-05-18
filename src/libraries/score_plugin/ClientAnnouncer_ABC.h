// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef __ClientAnnouncer_ABC_h_
#define __ClientAnnouncer_ABC_h_

#include <boost/noncopyable.hpp>
#include <boost/shared_ptr.hpp>

namespace plugins
{
namespace score
{
    class Score;
// =============================================================================
/** @class  ClientAnnouncer_ABC
    @brief  Client announcer interface
*/
// Created: SBO 2011-05-17
// =============================================================================
class ClientAnnouncer_ABC : private boost::noncopyable
{

public:
    //! @name Constructors/Destructor
    //@{
             ClientAnnouncer_ABC() {}
    virtual ~ClientAnnouncer_ABC() {}
    //@}

    //! @name Operations
    //@{
    virtual void Visit( boost::shared_ptr< Score > score ) = 0;
    virtual void Visit( const std::string& profile, boost::shared_ptr< Score > score ) = 0;
    //@}
};
}
}

#endif // __ClientAnnouncer_ABC_h_
