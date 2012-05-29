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

#include "clients_kernel/Usages.h"

namespace kernel
{
    class AccommodationTypes;
    class Controller;
    class Entity_ABC;
    class PropertiesDictionary;
}

namespace xml
{
    class xistream;
}

// =============================================================================
/** @class  Usages
    @brief  Usages
*/
// Created: LGY 2011-04-14
// =============================================================================
class Usages : public kernel::Usages
{
public:
    //! @name Constructors/Destructor
    //@{
             Usages( kernel::PropertiesDictionary& dictionary, const kernel::AccommodationTypes& accommodationTypes, float livingSpace, kernel::Entity_ABC& owner, kernel::Controller& controller );
             Usages( xml::xistream& xis, kernel::PropertiesDictionary& dictionary, const kernel::AccommodationTypes& accommodationTypes, float livingSpace, kernel::Entity_ABC& owner, kernel::Controller& controller );
    virtual ~Usages();
    //@}

    //! @name Serializable_ABC
    //@{
    virtual void SerializeAttributes( xml::xostream& ) const;
    //@}

private:
    //! @name Helpers
    //@{
    void ReadUsages( xml::xistream& xis );
    //@}
};

#endif // __Usages_h_
