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

#include <boost/filesystem/path.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/noncopyable.hpp>
#include <string>
#include <map>

namespace xml
{
    class xistream;
}

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
             PropagationManager();
    virtual ~PropagationManager();
    //@}

    //! @name Types
    //@{
    typedef std::map< double, std::string > T_Colors;
    typedef T_Colors::const_iterator      CIT_Colors;
    typedef std::vector< std::string >                    T_Files;
    typedef std::map< boost::posix_time::ptime, T_Files > T_Schedule;
    typedef T_Schedule::const_iterator                  CIT_Schedule;
    //@}

    //! @name Operations
    //@{
    void Initialize( const std::string& config );
    T_Files GetCurrentFiles( const std::string& time ) const;
    const std::string& GetProjectionFile() const;
    std::string GetColor( float value ) const;
    //@}

private:
    //! @name Helpers
    //@{
    void ReadFile( xml::xistream& xis, const boost::filesystem::path& path );
    void ReadColor( xml::xistream& xis );
    //@}

private:
    //! @name Member data
    //@{
    std::string projection_;
    T_Colors colors_;
    T_Schedule schedule_;
    //@}
};

#endif // PropagationManager_h
