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
#include <string>
#include <vector>
#include <boost/noncopyable.hpp>
#include <boost/shared_ptr.hpp>

namespace runtime
{

class Process_ABC;

// =============================================================================
/** @class  Runtime_ABC
    @brief  Runtime_ABC interface definition
*/
// Created: BAX 2012-03-07
// =============================================================================
class Runtime_ABC : public boost::noncopyable
{
public:
    //! @name Constructor/Destructor
    //@{
             Runtime_ABC() {}
    virtual ~Runtime_ABC() {}
    //@}

    //! @name Typedef helpers
    //@{
    typedef std::vector< boost::shared_ptr< Process_ABC > > T_Processes;
    //@}

    //! @name Operations
    //@{
    virtual T_Processes GetProcesses() const = 0;
    virtual boost::shared_ptr< Process_ABC > GetProcess( int pid ) const = 0;
    virtual boost::shared_ptr< Process_ABC > Start( const std::string& cmd,
                                                    const std::vector< std::string >& args,
                                                    const std::string& run ) const = 0;
    //@}
};

}

#endif
