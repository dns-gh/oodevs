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
#include <tools/StdFileWrapper.h>
#include <boost/noncopyable.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/ptr_container/ptr_deque.hpp>

namespace tools
{
    class LogFactory_ABC;

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
     RotatingLog( tools::LogFactory_ABC& factory, const tools::Path& filename, std::size_t files, std::size_t size, bool truncate );
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
    //! @name Types
    //@{
    class Stream;
    //@}

private:
    //! @name Helpers
    //@{
    void Populate();
    void DoWrite( const std::string& line );
    void Log( const std::string& line );
    void Rotate();
    void CreateLog();
    //@}

private:
    //! @name Member data
    //@{
    LogFactory_ABC& factory_;
    tools::Path filename_;
    std::size_t files_;
    std::streamoff size_, count_;
    boost::ptr_deque< Stream > logs_;
    //@}
};

}

#endif // tools_RotatingLog_h
