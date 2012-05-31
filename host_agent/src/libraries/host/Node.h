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

#include <boost/filesystem/path.hpp>
#include <boost/shared_ptr.hpp>

namespace boost
{
    class mutex;
    class shared_mutex;
}

namespace runtime
{
    class Runtime_ABC;
    class Process_ABC;
}

namespace host
{
    struct Async;
    class FileSystem_ABC;
    struct Package_ABC;
    struct PackageFactory_ABC;
    class Pool_ABC;
    class Port_ABC;
    class PortFactory_ABC;
    class UuidFactory_ABC;

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
             Node( const PackageFactory_ABC& packages, const FileSystem_ABC& system,
                   const UuidFactory_ABC& uuids, Pool_ABC& pool, const Path& root,
                   const std::string& name, PortFactory_ABC& ports );
             Node( const PackageFactory_ABC& packages, const FileSystem_ABC& system,
                   const UuidFactory_ABC& uuids, Pool_ABC& pool, const Tree& tree,
                   const runtime::Runtime_ABC& runtime, PortFactory_ABC& ports );
    virtual ~Node();
    //@}

    //! @name Node_ABC methods
    //@{
    virtual Uuid GetId() const;
    virtual int  GetPort() const;
    virtual std::string GetName() const;
    virtual Tree GetProperties() const;

    //@}

    //! @name Public methods
    //@{
    virtual Tree Save() const;
    virtual bool Start( const T_Starter& starter );
    virtual bool Stop();
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

private:
    //! @name Private methods
    //@{
    void ParsePackages( const Path& cache );
    void IdentifyPackages();
    Tree GetCommonProperties() const;
    //@}

private:
    //! @name Private members
    //@{
    const FileSystem_ABC& system_;
    const UuidFactory_ABC& uuids_;
    const PackageFactory_ABC& packages_;
    const Uuid id_;
    const std::string name_;
    const Path root_;
    const std::auto_ptr< boost::shared_mutex > access_;
    const std::auto_ptr< Port_ABC > port_;
    boost::shared_ptr< Package_ABC > install_;
    boost::shared_ptr< Package_ABC > cache_;
    T_Process process_;
    const std::auto_ptr< Async > async_;
    //@}
};
}

#endif // NODE_H