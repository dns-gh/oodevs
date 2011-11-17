// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef shield_Model_ABC_h
#define shield_Model_ABC_h

#include <boost/noncopyable.hpp>

namespace dispatcher
{
    class ClientPublisher_ABC;
}

namespace shield
{
// =============================================================================
/** @class  Model_ABC
    @brief  Model declaration
*/
// Created: MCO 2011-11-09
// =============================================================================
class Model_ABC : boost::noncopyable
{
public:
    //! @name Constructors/Destructor
    //@{
             Model_ABC() {}
    virtual ~Model_ABC() {}
    //@}

    //! @name Operations
    //@{
    virtual void Send( dispatcher::ClientPublisher_ABC& publisher ) const = 0;
    //@}
};

}

#endif // shield_Model_ABC_h
// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef shield_Model_ABC_h
#define shield_Model_ABC_h

#include <boost/noncopyable.hpp>

namespace dispatcher
{
    class ClientPublisher_ABC;
}

namespace shield
{
// =============================================================================
/** @class  Model_ABC
    @brief  Model declaration
*/
// Created: MCO 2011-11-09
// =============================================================================
class Model_ABC : boost::noncopyable
{
public:
    //! @name Constructors/Destructor
    //@{
             Model_ABC() {}
    virtual ~Model_ABC() {}
    //@}

    //! @name Operations
    //@{
    virtual void Send( dispatcher::ClientPublisher_ABC& publisher ) const = 0;
    //@}
};

}

#endif // shield_Model_ABC_h
