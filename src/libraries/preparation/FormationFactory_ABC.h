// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __FormationFactory_ABC_h_
#define __FormationFactory_ABC_h_

#include <boost/noncopyable.hpp>

namespace kernel
{
    class Entity_ABC;
    class Formation_ABC;
    class HierarchyLevel_ABC;
    class FormationLevels;
}

namespace xml
{
    class xistream;
}

// =============================================================================
/** @class  FormationFactory_ABC
    @brief  FormationFactory_ABC
*/
// Created: SBO 2006-09-19
// =============================================================================
class FormationFactory_ABC : private boost::noncopyable
{
public:
    //! @name Constructors/Destructor
    //@{
             FormationFactory_ABC() {};
    virtual ~FormationFactory_ABC() {};
    //@}

    //! @name Operations
    //@{
    virtual kernel::Formation_ABC* Create( kernel::Entity_ABC& parent, const kernel::HierarchyLevel_ABC& level, const kernel::FormationLevels& levels, const QString& name = "" ) = 0;
    virtual kernel::Formation_ABC* Create( xml::xistream& xis, kernel::Entity_ABC& parent, const kernel::FormationLevels& levels ) = 0;
    //@}
};

#endif // __FormationFactory_ABC_h_
