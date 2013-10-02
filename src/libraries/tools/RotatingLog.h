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
#include <boost/scoped_ptr.hpp>
#include <deque>

namespace tools
{
    class Log_ABC;
    class Ofstream;

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
     RotatingLog( Log_ABC& log, const Path& filename, std::size_t files, std::size_t size, bool truncate );
    ~RotatingLog();
    //@}

    //! @name Operations
    //@{
    void Write( const std::string& line );
    //@}

private:
    //! @name Helpers
    //@{
    void Populate();
    void Log( const std::string& line );
    void Prune();
    void Rotate();
    void CreateLog();
    //@}

private:
    //! @name Member data
    //@{
    Log_ABC& log_;
    Path filename_;
    std::size_t files_;
    std::streamoff size_, count_;
    std::deque< Path > history_;
    boost::scoped_ptr< Ofstream > stream_;
    //@}
};

}

#endif // tools_RotatingLog_h
