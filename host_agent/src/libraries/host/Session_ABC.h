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

#include <boost/noncopyable.hpp>
#include <boost/property_tree/ptree_fwd.hpp>
#include <boost/uuid/uuid.hpp>

namespace host
{

// =============================================================================
/** @class  Session_ABC
    @brief  Session_ABC interface
*/
// Created: BAX 2012-03-16
// =============================================================================
class Session_ABC : public boost::noncopyable
{
public:
    //! @name Constructors/Destructor
    //@{
             Session_ABC() {}
    virtual ~Session_ABC() {}
    //@}

    //! @name Methods
    //@{
    virtual boost::uuids::uuid GetId() const = 0;
    virtual boost::uuids::uuid GetNode() const = 0;
    virtual boost::property_tree::ptree GetProperties() const = 0;
    //@}
};

}

#endif // SESSION_ABC_H
