// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef __Usages_ABC_h_
#define __Usages_ABC_h_

#include "Extension_ABC.h"
#include "Displayable_ABC.h"
#include "Serializable_ABC.h"
#include <boost/noncopyable.hpp>

namespace kernel
{
// =============================================================================
/** @class  Usages_ABC
    @brief  Usages declaration
*/
// Created: LGY 2011-04-14
// =============================================================================
class Usages_ABC : public Extension_ABC
                 , public Displayable_ABC
                 , public Serializable_ABC
                 , private boost::noncopyable
{
public:
    //! @name Constructors/Destructor
    //@{
             Usages_ABC() {}
    virtual ~Usages_ABC() {}
    //@}

    //! @name Operations
    //@{
    virtual void Add( const std::string& usage, unsigned int proportion ) = 0;
    virtual unsigned int Find( const std::string& usage ) const = 0;
    //@}
};

}

#endif // __Usages_ABC_h_
