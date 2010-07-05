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

namespace kernel
{
    class Controllers;
}

class IdManager;

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
             FormationFactory( kernel::Controllers& controllers, IdManager& idManager );
    virtual ~FormationFactory();
    //@}

    //! @name Operations
    //@{
    virtual kernel::Formation_ABC* Create( kernel::Entity_ABC& parent, const kernel::HierarchyLevel_ABC& level, const QString& name = "" );
    virtual kernel::Formation_ABC* Create( xml::xistream& xis, kernel::Entity_ABC& parent, const kernel::FormationLevels& levels );
    //@}

private:
    //! @name Copy/Assignment
    //@{
    FormationFactory( const FormationFactory& );            //!< Copy constructor
    FormationFactory& operator=( const FormationFactory& ); //!< Assignment operator
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controllers& controllers_;
    IdManager& idManager_;
    //@}
};

#endif // __FormationFactory_h_
