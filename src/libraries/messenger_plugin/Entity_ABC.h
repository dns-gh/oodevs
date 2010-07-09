// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 MASA Group
//
// *****************************************************************************

#ifndef __Entity_ABC_h_
#define __Entity_ABC_h_

namespace kernel
{
    class CoordinateConverter_ABC;
}

namespace xml
{
    class xostream;
}

namespace plugins
{
namespace messenger
{

// =============================================================================
/** @class  Entity_ABC
    @brief  Entity_ABC
*/
// Created: RDS 2008-04-09
// =============================================================================
class Entity_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             Entity_ABC() {};
    virtual ~Entity_ABC() {};
    //@}

    //! @name xml read / write
    //@{
    virtual void Write( xml::xostream& xos, const kernel::CoordinateConverter_ABC& converter ) const = 0;
    //@}
};

}
}

#endif // __Entity_ABC_h_
