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

namespace cpplog
{
    class BaseLogger;
};

namespace runtime
{
    struct Api_ABC;

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
    explicit Runtime( cpplog::BaseLogger& log, const Api_ABC& api );
    virtual ~Runtime();
    //@}

    //! @name Operations
    //@{
    virtual boost::shared_ptr< Process_ABC > GetProcess( int pid ) const;
    virtual boost::shared_ptr< Process_ABC > Start( const std::string& cmd,
                                                    const std::vector< std::string >& args,
                                                    const std::string& run,
                                                    const std::string& log ) const;
    virtual boost::filesystem::path GetModuleFilename() const;
    virtual std::string GetLastError() const;
    //@}

private:
    cpplog::BaseLogger& log_;
    const Api_ABC& api_;
};
}

#endif
