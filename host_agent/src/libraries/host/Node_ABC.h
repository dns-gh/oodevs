// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#ifndef NODE_ABC_H
#define NODE_ABC_H

#include <boost/noncopyable.hpp>
#include <boost/uuid/uuid.hpp>
#include <string>

namespace host
{

// =============================================================================
/** @class  Node_ABC
    @brief  Node_ABC interface
*/
// Created: BAX 2012-04-03
// =============================================================================
class Node_ABC : public boost::noncopyable
{
public:
    //! @name Constructors/Destructor
    //@{
             Node_ABC() {}
    virtual ~Node_ABC() {}
    //@}

    //! @name Methods
    //@{
    virtual boost::uuids::uuid GetTag() const = 0;
    virtual std::string ToJson() const = 0;
    virtual void Start() = 0;
    virtual void Stop() = 0;
    //@}
};

}

#endif // NODE_ABC_H
