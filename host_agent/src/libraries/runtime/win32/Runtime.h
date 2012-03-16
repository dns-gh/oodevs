// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
#ifndef RUNTIME_H
#define RUNTIME_H
#include <runtime/Runtime_ABC.h>

namespace runtime
{

class Api_ABC;

// =============================================================================
/** @class  Runtime
    @brief  Runtime definition
*/
// Created: BAX 2012-03-07
// =============================================================================
class Runtime : public Runtime_ABC
{
public:
    //! @name Constructor/Destructor
    //@{
    explicit Runtime( const Api_ABC& api );
    virtual ~Runtime();
    //@}

    //! @name Operations
    //@{
    virtual T_Processes GetProcesses() const;
    virtual boost::shared_ptr< Process_ABC > GetProcess( int pid ) const;
    virtual boost::shared_ptr< Process_ABC > Start( const std::string& cmd,
                                                    const std::vector< std::string >& args,
                                                    const std::string& run ) const;
    //@}

private:
    const Api_ABC& api_;
};

}

#endif
