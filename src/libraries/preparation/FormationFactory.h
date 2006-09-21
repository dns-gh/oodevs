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
    explicit FormationFactory( kernel::Controllers& controllers );
    virtual ~FormationFactory();
    //@}

    //! @name Operations
    //@{
    virtual kernel::Formation_ABC* Create( kernel::Entity_ABC& parent, const QString& level );
    //@}

private:
    //! @name Copy/Assignement
    //@{
    FormationFactory( const FormationFactory& );            //!< Copy constructor
    FormationFactory& operator=( const FormationFactory& ); //!< Assignement operator
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controllers& controllers_;
    //@}
};

#endif // __FormationFactory_h_
