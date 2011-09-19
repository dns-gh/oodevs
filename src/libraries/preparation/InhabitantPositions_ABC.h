// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef __InhabitantPositions_ABC_h_
#define __InhabitantPositions_ABC_h_

#include "clients_kernel/Positions.h"
#include <boost/noncopyable.hpp>

namespace kernel
{
    class Location_ABC;
}

// =============================================================================
/** @class  InhabitantPositions_ABC
    @brief  Inhabitant positions declaration
*/
// Created: LGY 2011-09-19
// =============================================================================
class InhabitantPositions_ABC : public kernel::Positions
                              , private boost::noncopyable
{
public:
    //! @name Constructors/Destructor
    //@{
             InhabitantPositions_ABC() {}
    virtual ~InhabitantPositions_ABC() {}
    //@}

    //! @name Operations
    //@{
    virtual void Add( const kernel::Location_ABC& location ) = 0;
    virtual void Remove( const kernel::Location_ABC& location ) = 0;
    //@}
};

#endif // __InhabitantPositions_ABC_h_
