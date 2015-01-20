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
             PropagationManager( const tools::Path& config, const std::string& time );
    virtual ~PropagationManager();
    //@}

    //! @name Operations
    //@{
    boost::shared_ptr< Extractor_ABC > CreateExtractor( const tools::Path& file ) const;
    tools::Path::T_Paths GetFiles( const std::string& time ) const;
    const tools::Path& GetProjectionFile() const;
    //@}

private:
    //! @name Member data
    //@{
    tools::Path projection_;
    boost::optional< short > timeZone_;
    std::map< boost::posix_time::ptime, tools::Path::T_Paths > schedule_;
    //@}
};

#endif // PropagationManager_h
