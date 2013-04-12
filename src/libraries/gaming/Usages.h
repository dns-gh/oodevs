// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef __Usages_h_
#define __Usages_h_

#include "clients_gui/Usages.h"

namespace kernel
{
    class AccommodationTypes;
    class Controller;
    class Entity_ABC;
}

namespace gui
{
    class PropertiesDictionary;
}

namespace sword
{
    class UrbanAttributes;
}

// =============================================================================
/** @class  Usages
    @brief  Usages
*/
// Created: LGY 2011-04-14
// =============================================================================
class Usages : public gui::Usages
{
public:
    //! @name Constructors/Destructor
    //@{
             Usages( const sword::UrbanAttributes& message, gui::PropertiesDictionary& dictionary, const kernel::AccommodationTypes& accommodationTypes,
                     float livingSpace, kernel::Entity_ABC& owner );
    virtual ~Usages();
    //@}
};

#endif // __Usages_h_
