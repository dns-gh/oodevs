// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __Objects_h_
#define __Objects_h_

#include "clients_kernel/Serializable_ABC.h"
#include "clients_kernel/Extension_ABC.h"
#include <tools/Resolver.h>
#include <boost/noncopyable.hpp>

class Object;

// =============================================================================
/** @class  Objects
    @brief  Objects
*/
// Created: JSR 2011-02-22
// =============================================================================
class Objects : public kernel::Extension_ABC
              , public kernel::Serializable_ABC
              , public tools::Resolver< const Object >
              , private boost::noncopyable
{
public:
    //! @name Constructors/Destructor
    //@{
             Objects();
    virtual ~Objects();
    //@}

    //! @name Operations
    //@{
    void AddObject( const Object& object );
    //@}

private:
    //! @name Helpers
    //@{
    virtual void SerializeAttributes( xml::xostream& ) const;
    //@}
};

#endif // __Objects_h_
