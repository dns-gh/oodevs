// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#ifndef __Host_ABC_h_
#define __Host_ABC_h_

#include <boost/noncopyable.hpp>
#include <string>
#include <map>

namespace frontend
{
// =============================================================================
/** @class  Host_ABC
    @brief  Host_ABC
*/
// Created: SBO 2010-10-21
// =============================================================================
class Host_ABC : private boost::noncopyable
{
public:
    //! @name Constructors/Destructor
    //@{
    typedef std::map< std::string, std::string > T_Parameters;
    //@}

public:
    //! @name Constructors/Destructor
    //@{
             Host_ABC() {}
    virtual ~Host_ABC() {}
    //@}

    //! @name Operations
    //@{
    virtual std::string GetId() const = 0;
    virtual void StartSimulation( const tools::Path& exercise, const tools::Path& session ) const = 0;
    virtual void StartDispatcher( const tools::Path& exercise, const tools::Path& session, const T_Parameters& parameters ) const = 0;
    virtual void StartReplay( const tools::Path& exercise, const tools::Path& session ) const = 0;
    virtual void StopSession( const tools::Path& exercise, const tools::Path& session ) const = 0;
    virtual void QueryProfileList( const tools::Path& exercise ) const = 0;
    //@}
};

}

#endif // __Host_ABC_h_
