// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __FormationFactory_h_
#define __FormationFactory_h_

#include "FormationFactory_ABC.h"
#include "ENT/ENT_Enums.h"

namespace kernel
{
    class Controllers;
    class SymbolFactory;
}

namespace tools
{
    class IdManager;
}

class StaticModel;

// =============================================================================
/** @class  FormationFactory
    @brief  FormationFactory
*/
// Created: SBO 2006-09-19
// =============================================================================
class FormationFactory : public FormationFactory_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             FormationFactory( kernel::Controllers& controllers, const StaticModel& staticModel, tools::IdManager& idManager, kernel::SymbolFactory& symbolsFactory );
    virtual ~FormationFactory();
    //@}

    //! @name Operations
    //@{
    virtual kernel::Formation_ABC* Create( kernel::Entity_ABC& parent, E_NatureLevel level, const QString& name = "" );
    virtual kernel::Formation_ABC* Create( xml::xistream& xis, kernel::Entity_ABC& parent );
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controllers& controllers_;
    const StaticModel& staticModel_;
    tools::IdManager& idManager_;
    kernel::SymbolFactory& symbolsFactory_;
    //@}
};

#endif // __FormationFactory_h_
