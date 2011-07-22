// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#ifndef __CreationListener_ABC_h_
#define __CreationListener_ABC_h_

#include <boost/shared_ptr.hpp>

namespace sword
{
    class Listener;
}
namespace actions
{

// =============================================================================
/** @class  CreationListener_ABC
    @brief  CreationListener_ABC
*/
// Created: HBD 2010-12-10
// =============================================================================
class CreationListener_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
    CreationListener_ABC() {};
     virtual ~CreationListener_ABC() {};
    //@}

    //! @name Operations
    //@{
    virtual void RegisterListener( boost::shared_ptr< sword::Listener >& listener ) = 0;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    CreationListener_ABC( const CreationListener_ABC& );            //!< Copy constructor
    CreationListener_ABC& operator=( const CreationListener_ABC& ); //!< Assignment operator
    //@}
};
}

#endif // __CreationListener_ABC_h_
