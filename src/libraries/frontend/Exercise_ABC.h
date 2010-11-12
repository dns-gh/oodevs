// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#ifndef __Exercise_ABC_h_
#define __Exercise_ABC_h_

#include <boost/noncopyable.hpp>

namespace frontend
{

// =============================================================================
/** @class  Exercise_ABC
    @brief  Exercise_ABC
*/
// Created: SBO 2010-10-21
// =============================================================================
class Exercise_ABC : private boost::noncopyable
{

public:
    //! @name Constructors/Destructor
    //@{
             Exercise_ABC() {}
    virtual ~Exercise_ABC() {}
    //@}

    //! @name Accessors
    //@{
    virtual std::string GetId() const = 0;
    virtual std::string GetName() const = 0;
    virtual unsigned int GetPort() const = 0;
    virtual bool IsRunning() const = 0;
    virtual bool IsHostedBy( const std::string& host ) const = 0;
    virtual void StartSession( const std::string& session ) const = 0;
    virtual void StopSession( const std::string& session ) const = 0;
    //@}

    //! @name Operations
    //@{
    virtual void SetRunning( bool running ) = 0;
    //@}
};

}

#endif // __Exercise_ABC_h_
