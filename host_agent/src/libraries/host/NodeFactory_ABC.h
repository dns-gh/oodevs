// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#ifndef NODE_FACTORY_ABC_H
#define NODE_FACTORY_ABC_H

#include <boost/filesystem.hpp>
#include <boost/noncopyable.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/uuid/uuid.hpp>
#include <map>

namespace host
{
    class Node_ABC;

// =============================================================================
/** @class  NodeFactory_ABC
    @brief  NodeFactory_ABC interface
*/
// Created: BAX 2012-04-03
// =============================================================================
class NodeFactory_ABC : public boost::noncopyable
{
public:
    //! @name Constructors/Destructor
    //@{
             NodeFactory_ABC() {}
    virtual ~NodeFactory_ABC() {}
    //@}

    //! @name Type helpers
    //@{
    typedef std::map< boost::uuids::uuid, boost::shared_ptr< Node_ABC > > T_Nodes;
    //@}

    //! @name Methods
    //@{
    virtual boost::shared_ptr< Node_ABC > Create( const std::string& name ) const = 0;
    virtual T_Nodes Reload() const = 0;
    //@}
};

}

#endif // NODE_FACTORY_ABC_H
