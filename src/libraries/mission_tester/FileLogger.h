// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef __FileLogger_h_
#define __FileLogger_h_

#include "Logger.h"
#include <boost/utility/base_from_member.hpp>

namespace mission_tester
{
// =============================================================================
/** @class  FileLogger
    @brief  FileLogger
*/
// Created: PHC 2011-04-07
// =============================================================================
class FileLogger : private boost::base_from_member< std::auto_ptr< std::ostream > >
                 , public Logger
{
public:
    //! @name Constructors/Destructor
    //@{
    explicit FileLogger( const std::string& filename );
    virtual ~FileLogger();
    //@}

private:
    //! @name Types
    //@{
    typedef boost::base_from_member< std::auto_ptr< std::ostream > > T_Stream;
    //@}
};
}

#endif // __FileLogger_h_
