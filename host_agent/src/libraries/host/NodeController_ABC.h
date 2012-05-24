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
#include <boost/shared_ptr.hpp>
#include <boost/uuid/uuid.hpp>
#include <vector>

namespace host
{
    struct Node_ABC;
    typedef boost::property_tree::ptree Tree;
    typedef boost::uuids::uuid Uuid;

// =============================================================================
/** @class  NodeController_ABC
    @brief  NodeController_ABC interface
*/
// Created: BAX 2012-04-03
// =============================================================================
class NodeController_ABC : public boost::noncopyable
{
public:
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
    //@}

    //! @name Methods
    //@{
    virtual void    Reload() = 0;
    virtual T_Nodes List( int offset, int limit ) const = 0;
    virtual size_t  Count() const = 0;
    virtual bool    Has( const Uuid& id ) const = 0;
    virtual T_Node  Get( const Uuid& id ) const = 0;
    virtual T_Node  Create( const std::string& name ) = 0;
    virtual T_Node  Delete( const Uuid& id ) = 0;
    virtual T_Node  Start( const Uuid& id ) const = 0;
    virtual T_Node  Stop( const Uuid& id ) const = 0;
    virtual Tree    UploadPack( const Uuid& id, std::istream& src ) const = 0;
    virtual Tree    GetPack( const Uuid& id ) const = 0;
    virtual Tree    DeletePack( const Uuid& id ) = 0;
    //@}
};

}

#endif // NODE_CONTROLLER_ABC_H
