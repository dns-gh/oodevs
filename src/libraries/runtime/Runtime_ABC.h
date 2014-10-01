// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
#ifndef RUNTIME_ABC_H
#define RUNTIME_ABC_H

#include <boost/noncopyable.hpp>
#include <string>
#include <vector>

namespace boost
{
    template< typename T > class shared_ptr;
namespace filesystem
{
    class path;
}
}

namespace runtime
{
    struct Process_ABC;

// =============================================================================
/** @class  Runtime_ABC
    @brief  Runtime_ABC interface definition
*/
// Created: BAX 2012-03-07
// =============================================================================
struct Runtime_ABC : public boost::noncopyable
{
    //! @name Constructor/Destructor
    //@{
             Runtime_ABC() {}
    virtual ~Runtime_ABC() {}
    //@}

    //! @name Operations
    //@{
    virtual boost::shared_ptr< Process_ABC > GetProcess( int pid ) const = 0;
    virtual boost::shared_ptr< Process_ABC > Start( const std::string& cmd,
                                                    const std::vector< std::string >& args,
                                                    const std::string& run,
                                                    const std::string& log ) const = 0;
    virtual boost::filesystem::path GetModuleFilename() const = 0;
    virtual std::string GetLastError() const = 0;
    //@}
};
}

#endif
