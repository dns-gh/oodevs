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
#include <boost/shared_ptr.hpp>
#include <boost/uuid/uuid.hpp>
#include <vector>

namespace host
{
    class Node_ABC;

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
    virtual bool    Has( const boost::uuids::uuid& id ) const = 0;
    virtual T_Node  Get( const boost::uuids::uuid& id ) const = 0;
    virtual T_Node  Create( const std::string& name ) = 0;
    virtual T_Node  Delete( const boost::uuids::uuid& id ) = 0;
    virtual T_Node  Start( const boost::uuids::uuid& id ) const = 0;
    virtual T_Node  Stop( const boost::uuids::uuid& id ) const = 0;
    //@}
};

}

#endif // NODE_CONTROLLER_ABC_H
