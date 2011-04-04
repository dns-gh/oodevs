// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef __PositionsPlugin_h_
#define __PositionsPlugin_h_

#pragma warning( disable: 4996 )
#include "dispatcher/Plugin_ABC.h"
#include <boost/noncopyable.hpp>
#include <boost/filesystem/fstream.hpp>
#include <string>
#include <vector>
#include <qdatetime.h>
#include "TeamResolver.h"

class QDateTime;

namespace dispatcher
{ 
    class Config;
}

namespace plugins
{
namespace positions
{

// =============================================================================
/** @class  PositionsPlugin
    @brief  PositionsPlugin
*/
// Created: ABR 2011-04-01
// =============================================================================
class PositionsPlugin : public dispatcher::Plugin_ABC
                      , private boost::noncopyable
{

public:
    //! @name Constructors/Destructor
    //@{
             PositionsPlugin( const dispatcher::Config& config, int exportFrequency );
    virtual ~PositionsPlugin();
    //@}

    //! @name Operations
    //@{
    virtual void NotifyClientAuthenticated( dispatcher::ClientPublisher_ABC& client, dispatcher::Profile_ABC& profile );
    virtual void NotifyClientLeft( dispatcher::ClientPublisher_ABC& client );
    virtual void Receive( const sword::SimToClient& message );
    //@}

private:
    //! @name Helpers
    //@{
    void Export();
    //@}

    //! @name Types
    //@{
    typedef std::vector< std::string >  T_Times;
    typedef T_Times::const_iterator   CIT_Times;
    //@}

private:
    //! @name Member data
    //@{
    boost::filesystem::path     filepath_;
    int                         exportFrequency_;
    bool                        firstTick_;
    QDateTime                   lastExportTime_;
    QDateTime                   currentTime_;
    TeamResolver                teams_;
    T_Times                     times_;
    //@}

public:
    //! @name Public Static Member data
    //@{
    static const std::string    separator_;
    //@}
};

} // Namespace positions

} // Namespace plugins

#endif // __PositionsPlugin_h_
