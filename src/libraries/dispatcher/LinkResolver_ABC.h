// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __LinkResolver_ABC_h_
#define __LinkResolver_ABC_h_

#include <boost/noncopyable.hpp>
#include <string>

namespace dispatcher
{
    class ClientPublisher_ABC;

// =============================================================================
/** @class  LinkResolver_ABC
    @brief  Link resolver declaration
*/
// Created: AGE 2007-08-24
// =============================================================================
class LinkResolver_ABC : boost::noncopyable
{
public:
    //! @name Constructors/Destructor
    //@{
             LinkResolver_ABC() {}
    virtual ~LinkResolver_ABC() {}
    //@}

    //! @name Operations
    //@{
    virtual ClientPublisher_ABC& GetConnectedPublisher( const std::string& link ) const = 0;
    //@}
};

}

#endif // __LinkResolver_ABC_h_
