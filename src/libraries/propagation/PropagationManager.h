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

namespace xml
{
    class xistream;
}

// =============================================================================
/** @class  PropagationManager
    @brief  Propagation manager
*/
// Created: LGY 2012-11-07
// =============================================================================
class PropagationManager : private boost::noncopyable
{
public:
    //! @name Constructors/Destructor
    //@{
             PropagationManager();
    virtual ~PropagationManager();
    //@}

    //! @name Types
    //@{
    typedef std::map< boost::posix_time::ptime, tools::Path::T_Paths > T_Schedule;
    //@}

    //! @name Operations
    //@{
    void Initialize( const tools::Path& config, const std::string& time );
    boost::shared_ptr< Extractor_ABC > CreateExtractor( const tools::Path& file ) const;
    tools::Path::T_Paths GetFiles( const std::string& time );
    const tools::Path& GetProjectionFile() const;
    //@}

private:
    //! @name Helpers
    //@{
    void ReadFile( xml::xistream& xis, const tools::Path& path, const boost::posix_time::ptime& startTime, boost::posix_time::time_duration& delta );
    void ReadColor( xml::xistream& xis );
    //@}

private:
    //! @name Member data
    //@{
    tools::Path projection_;
    boost::optional< short > timeZone_;
    T_Schedule schedule_;
    //@}
};

#endif // PropagationManager_h
