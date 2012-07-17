// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#ifndef NODE_CONTROLLER_ABC_H
#define NODE_CONTROLLER_ABC_H

#include <boost/noncopyable.hpp>
#include <boost/property_tree/ptree_fwd.hpp>
#include <vector>

namespace boost
{
    template< typename T > class shared_ptr;
namespace filesystem
{
    class path;
}
namespace uuids
{
    struct uuid;
}
}

namespace host
{
    struct Node_ABC;
    typedef boost::filesystem::path Path;
    typedef boost::property_tree::ptree Tree;
    typedef boost::uuids::uuid Uuid;

// =============================================================================
/** @class  NodeController_ABC
    @brief  NodeController_ABC interface
*/
// Created: BAX 2012-04-03
// =============================================================================
struct NodeController_ABC : public boost::noncopyable
{
    //! @name Constructors/Destructor
    //@{
             NodeController_ABC() {}
    virtual ~NodeController_ABC() {}
    //@}

    //! @name Type helpers
    //@{
    typedef Node_ABC T_Base;
    typedef boost::shared_ptr< T_Base > T_Node;
    typedef std::vector< T_Node > T_Nodes;
    typedef std::vector< std::string > T_Exercises;
    //@}

    //! @name Methods
    //@{
    virtual void    Reload() = 0;
    virtual T_Nodes List( int offset, int limit ) const = 0;
    virtual size_t  Count() const = 0;
    virtual bool    Has( const Uuid& id ) const = 0;
    virtual T_Node  Get( const Uuid& id ) const = 0;
    virtual T_Node  Create( const std::string& name, size_t max, size_t parallel ) = 0;
    virtual T_Node  Delete( const Uuid& id ) = 0;
    virtual T_Node  Start( const Uuid& id ) const = 0;
    virtual T_Node  Stop( const Uuid& id ) const = 0;
    virtual T_Node  Update( const Uuid& id, size_t max, size_t parallel ) = 0;
    //@}

    //! @name Install Methods
    //@{
    virtual Tree    GetInstall( const Uuid& id ) const = 0;
    virtual Tree    DeleteInstall( const Uuid& id, const std::vector< size_t >& list ) = 0;
    //@}

    //! @name Cache Methods
    //@{
    virtual Tree    UploadCache( const Uuid& id, std::istream& src ) const = 0;
    virtual Tree    GetCache( const Uuid& id ) const = 0;
    virtual Tree    DeleteCache( const Uuid& id ) = 0;
    virtual Tree    InstallFromCache( const Uuid& id, const std::vector< size_t >& list ) = 0;
    //@}

    //! @name Exercise Methods
    //@{
    virtual T_Exercises GetExercises( const Uuid& id, int offset, int limit ) const = 0;
    virtual size_t      CountExercises( const Uuid& id ) const = 0;
    //@}
};
}

#endif // NODE_CONTROLLER_ABC_H
