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
             Host_ABC() {}
    virtual ~Host_ABC() {}
    //@}

    //! @name Operations
    //@{
    virtual std::string GetId() const = 0;
    virtual void StartSimulation( const std::string& exercise, const std::string& session ) const = 0;
    virtual void StartDispatcher( const std::string& exercise, const std::string& session ) const = 0;
    virtual void StartReplay( const std::string& exercise, const std::string& session ) const = 0;
    virtual void StopSession( const std::string& exercise, const std::string& session ) const = 0;
    virtual void QueryProfileList(const std::string& exercise) const = 0;
    virtual void Pause( const std::string& exercise, const std::string& session) const = 0;
    virtual void Resume( const std::string& exercise, const std::string& session) const = 0;
    virtual void SaveCheckpoint( const std::string& exercise, const std::string& session, const std::string& name) const = 0;
    //@}
};

}

#endif // __Host_ABC_h_
