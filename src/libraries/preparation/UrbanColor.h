// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef __UrbanColor_h_
#define __UrbanColor_h_

#include "clients_kernel/Serializable_ABC.h"
#include "clients_kernel/UrbanColor_ABC.h"

namespace kernel
{
    class Controllers;
    class Entity_ABC;
    class UrbanObject_ABC;
}

namespace gui
{
    class PropertiesDictionary;
}

namespace xml
{
    class xistream;
}

// =============================================================================
/** @class  UrbanColor
    @brief  UrbanColor
*/
// Created: LGY 2011-04-19
// =============================================================================
class UrbanColor : public kernel::UrbanColor_ABC
                 , public kernel::Serializable_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             UrbanColor( const kernel::Entity_ABC* parent, kernel::UrbanObject_ABC& object, gui::PropertiesDictionary& dictionary );
             UrbanColor( xml::xistream& xis, kernel::UrbanObject_ABC& object, gui::PropertiesDictionary& dictionary );
    virtual ~UrbanColor();
    //@}

    //! @name Serializable_ABC
    //@{
    virtual void SerializeAttributes( xml::xostream& ) const;
    //@}
};

#endif // __UrbanColor_h_
