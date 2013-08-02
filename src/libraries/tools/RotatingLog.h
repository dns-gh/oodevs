 // *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef tools_RotatingLog_h
#define tools_RotatingLog_h

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
     RotatingLog( tools::LogFactory_ABC& factory, const tools::Path& filename, std::size_t files, std::size_t size );
    ~RotatingLog();
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
    //! @name Helpers
    //@{
    void DoWrite( const std::string& line );

    std::size_t FindOldestFile() const;
    tools::Path GetFileName( std::size_t file ) const;
    //@}

private:
    //! @name Member data
    //@{
    LogFactory_ABC& factory_;
    tools::Path filename_;
    tools::Path extension_;
    std::size_t file_, files_;
    std::size_t size_, count_;
    std::auto_ptr< Log_ABC > pLog_;
    bool sizeInBytes_;
    //@}
};

}

#endif // tools_RotatingLog_h
