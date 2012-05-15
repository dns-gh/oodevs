// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#ifndef __UrbanFactory_ABC_h_
#define __UrbanFactory_ABC_h_

#include <boost/noncopyable.hpp>

namespace xml
{
    class xistream;
}

namespace gui
{
    class TerrainObjectProxy;
}

// =============================================================================
/** @class  UrbanFactory_ABC
    @brief  Urban factory declaration
*/
// Created: LGY 2012-04-10
// =============================================================================
class UrbanFactory_ABC : private boost::noncopyable
{
public:
    //! @name Constructors/Destructor
    //@{
             UrbanFactory_ABC() {}
    virtual ~UrbanFactory_ABC() {}
    //@}

    //! @name Operations
    //@{
    virtual gui::TerrainObjectProxy* Create( xml::xistream& xis, gui::TerrainObjectProxy* parent ) const = 0;
    //@}
};

#endif // __UrbanFactory_ABC_h_
