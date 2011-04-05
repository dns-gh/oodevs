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

#include "TeamResolver.h"
#include "dispatcher/Plugin_ABC.h"
#pragma warning( push, 0 )
#include <qdatetime.h>
#pragma warning( pop )
#include <boost/filesystem/path.hpp>
#include <string>
#include <vector>

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
{
public:
    //! @name Constructors/Destructor
    //@{
             PositionsPlugin( const std::string& filename, int exportFrequency );
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
    void SaveTime();
    void ExportData() const;
    //@}

private:
    //! @name Types
    //@{
    typedef std::vector< std::string >  T_Times;
    typedef T_Times::const_iterator   CIT_Times;
    //@}

private:
    //! @name Member data
    //@{
    boost::filesystem::path filepath_;
    int                     exportFrequency_;
    bool                    firstTick_;
    QDateTime               lastExportTime_;
    QDateTime               currentTime_;
    TeamResolver            teams_;
    T_Times                 times_;
    //@}

public:
    //! @name Member data
    //@{
    static const std::string separator_;
    //@}
};

}
}

#endif // __PositionsPlugin_h_
