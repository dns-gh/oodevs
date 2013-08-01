 // *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef __dispatcher_RotatingLog_h_
#define __dispatcher_RotatingLog_h_

#include <tools/Path.h>
#include <boost/noncopyable.hpp>
#include <boost/lexical_cast.hpp>
#include <memory>

namespace tools
{
    class LogFactory_ABC;
    class Log_ABC;

// =============================================================================
/** @class  RotatingLog
    @brief  Rotating log implementation
*/
// Created: MCO 2011-06-26
// =============================================================================
class RotatingLog : private boost::noncopyable
{
public:
    //! @name Constructors/Destructor
    //@{
    RotatingLog( tools::LogFactory_ABC& factory, const tools::Path& filename, unsigned int files, unsigned int size, bool sizeInBytes = false );
    //@}

    //! @name Operations
    //@{
    template< typename Serializable >
    void Write( const Serializable& s )
    {
        if( files_ > 0 )
            DoWrite( boost::lexical_cast< std::string >( s ) );
    }
    //@}

private:
    //! @name Operations
    //@{
    void DoWrite( const std::string& line );
    //@}

    //! @name Member data
    //@{
    LogFactory_ABC& factory_;
    tools::Path filename_;
    tools::Path fileNameNoExtension_;
    tools::Path extension_;
    unsigned int file_, files_, size_, count_;
    std::auto_ptr< Log_ABC > pLog_;
    bool sizeInBytes_;
    //@}
};

}

#endif // __dispatcher_RotatingLog_h_
