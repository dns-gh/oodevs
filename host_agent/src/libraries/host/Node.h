// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#ifndef NODE_H
#define NODE_H

#include "Node_ABC.h"

#include "runtime/Async.h"
#include <boost/filesystem/path.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/thread/shared_mutex.hpp>

namespace boost
{
    class mutex;
    class shared_mutex;
}

namespace runtime
{
    struct FileSystem_ABC;
    struct Pool_ABC;
    struct Process_ABC;
    struct Runtime_ABC;
}

namespace host
{
    struct Package_ABC;
    struct PackageFactory_ABC;
    struct Port_ABC;
    struct PortFactory_ABC;
    struct UuidFactory_ABC;
    typedef boost::shared_ptr< const Port_ABC > Port;

// =============================================================================
/** @class  Node_ABC
    @brief  Node_ABC interface
*/
// Created: BAX 2012-04-03
// =============================================================================
class Node : public Node_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             Node( const PackageFactory_ABC& packages, const runtime::FileSystem_ABC& system,
                   const UuidFactory_ABC& uuids, runtime::Pool_ABC& pool, const Path& root,
                   const std::string& name, PortFactory_ABC& ports );
             Node( const PackageFactory_ABC& packages, const runtime::FileSystem_ABC& system,
                   const UuidFactory_ABC& uuids, runtime::Pool_ABC& pool, const Path& root,
                   const Tree& tree, const runtime::Runtime_ABC& runtime, PortFactory_ABC& ports );
    virtual ~Node();
    //@}

    //! @name Node_ABC methods
    //@{
    virtual Uuid GetId() const;
    virtual Path GetRoot() const;
    virtual int  GetPort() const;
    virtual std::string GetName() const;
    virtual Tree GetProperties() const;

    //@}

    //! @name Public methods
    //@{
    virtual Tree Save() const;
    virtual bool Start( const T_Starter& starter, bool weak );
    virtual bool Stop( bool weak );
    //@}

    //! @name Node_ABC methods
    //@{
    virtual Tree GetInstall() const;
    virtual Tree DeleteInstall( const std::vector< size_t >& list );
    //@}

    //! @name Node_ABC methods
    //@{
    virtual void UploadCache( std::istream& src );
    virtual Tree GetCache() const;
    virtual Tree DeleteCache();
    virtual Tree InstallFromCache( const std::vector< size_t >& list );
    //@}

    //! @name Exercise methods
    //@{
    virtual T_Exercises GetExercises( int offset, int limit ) const;
    virtual size_t      CountExercises() const;
    virtual Tree        LinkExercise( const std::string& name ) const;
    virtual Tree        LinkExercise( const Tree& tree ) const;
    virtual void        UnlinkExercise( const Tree& tree ) const;
    //@}

private:
    //! @name Private methods
    //@{
    void ParsePackages( const Path& cache );
    Tree GetCommonProperties() const;
    //@}

private:
    //! @name Private constant members
    //@{
    const runtime::FileSystem_ABC& system_;
    const UuidFactory_ABC& uuids_;
    const PackageFactory_ABC& packages_;
    const Uuid id_;
    const std::string name_;
    const Path root_;
    const Port port_;
    //@}

    //! @name Private members
    //@{
    mutable boost::shared_mutex access_;
    boost::shared_ptr< Package_ABC > install_;
    boost::shared_ptr< Package_ABC > cache_;
    T_Process process_;
    bool stopped_;
    mutable runtime::Async async_;
    //@}
};
}

#endif // NODE_H