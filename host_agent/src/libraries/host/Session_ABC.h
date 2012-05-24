// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#ifndef SESSION_ABC_H
#define SESSION_ABC_H

#include <boost/function.hpp>
#include <boost/noncopyable.hpp>
#include <boost/property_tree/ptree_fwd.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/uuid/uuid.hpp>

namespace runtime
{
    class Process_ABC;
}

namespace host
{
    typedef boost::property_tree::ptree Tree;
    typedef boost::uuids::uuid Uuid;

// =============================================================================
/** @class  Session_ABC
    @brief  Session_ABC interface
*/
// Created: BAX 2012-03-16
// =============================================================================
struct Session_ABC : public boost::noncopyable
{
    //! @name Constructors/Destructor
    //@{
             Session_ABC() {}
    virtual ~Session_ABC() {}
    //@}

    //! @name Typedef helpers
    //@{
    typedef boost::shared_ptr< runtime::Process_ABC > T_Process;
    typedef boost::function< T_Process( const Session_ABC& ) > T_Starter;
    //@}

    //! @name Methods
    //@{
    virtual Uuid GetId() const = 0;
    virtual Uuid GetNode() const = 0;
    virtual std::string GetExercise() const = 0;
    virtual std::string GetName() const = 0;
    virtual int GetPort() const = 0;
    virtual std::string GetConfiguration() const = 0;
    virtual Tree GetProperties() const = 0;
    //@}

    //! @name Public methods
    //@{
    virtual Tree Save() const = 0;
    virtual bool Start( const T_Starter& starter ) = 0;
    virtual bool Stop() = 0;
    //@}
};

// =============================================================================
/** @class  SessionFactory_ABC
    @brief  SessionFactory_ABC interface
*/
// Created: BAX 2012-05-23
// =============================================================================
struct SessionFactory_ABC : public boost::noncopyable
{
    //! @name Constructors/Destructor
    //@{
             SessionFactory_ABC() {}
    virtual ~SessionFactory_ABC() {}
    //@}

    //! @name Typedef helpers
    //@{
    typedef boost::shared_ptr< Session_ABC > Ptr;
    //@}

    //! @name Methods
    //@{
    virtual Ptr Make( const Uuid& node, const std::string& name, const std::string& exercise ) const = 0;
    virtual Ptr Make( const Tree& tree ) const = 0;
    //@}
};


}

#endif // SESSION_ABC_H
