// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#ifndef PropagationManager_h
#define PropagationManager_h

#include <tools/Path.h>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/noncopyable.hpp>
#include <boost/optional.hpp>
#include <string>
#include <map>

class Extractor_ABC;

// PropagationManager handles a collection of propagation data files and the
// way they are scheduled and projected.
class PropagationManager : private boost::noncopyable
{
public:
             PropagationManager( const tools::Path& config, const std::string& time );
    virtual ~PropagationManager();

    // GetProjectionFile returns the path to the propagation data files
    // projection file.
    const tools::Path& GetProjectionFile() const;

    // GetFiles returns the set of propagation files activated at the closest
    // date preceding supplied time.
    tools::Path::T_Paths GetFiles( const std::string& time ) const;

    // CreateExtractor returns a data reader for specified propagation data
    // file.
    boost::shared_ptr< Extractor_ABC > CreateExtractor( const tools::Path& file ) const;

private:
    tools::Path projection_;
    boost::optional< short > timeZone_;
    std::map< boost::posix_time::ptime, tools::Path::T_Paths > schedule_;
};

#endif // PropagationManager_h
