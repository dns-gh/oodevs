// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef __ScoreAnnouncer_h_
#define __ScoreAnnouncer_h_

#include <boost/shared_ptr.hpp>
#include <boost/noncopyable.hpp>
#include <map>
#include <vector>

namespace dispatcher
{
    class ClientPublisher_ABC;
    class Profile_ABC;
}

namespace plugins
{
namespace score
{
class Score;

// =============================================================================
/** @class  ScoreAnnouncer
    @brief  ScoreAnnouncer
*/
// Created: SBO 2011-05-17
// =============================================================================
class ScoreAnnouncer : public boost::noncopyable
{

public:
    //! @name Constructors/Destructor
    //@{
             ScoreAnnouncer();
    virtual ~ScoreAnnouncer();
    //@}

    //! @name Operations
    //@{
    void Announce( dispatcher::ClientPublisher_ABC& client, dispatcher::Profile_ABC& profile );
    virtual void Visit( boost::shared_ptr< Score > score );
    virtual void Visit( const std::string& profile, const boost::shared_ptr< Score >& score );
    //@}

private:
    //! @name Types
    //@{
    typedef boost::shared_ptr< Score > T_Score;
    typedef std::map< std::string, std::vector< T_Score > > T_ProfileScores;
    //@}

private:
    //! @name Member data
    //@{
    T_ProfileScores profileScores_;
    std::vector< T_Score > everybodyScores_;
    //@}
};

}
}

#endif // __ScoreAnnouncer_h_
