// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __FormationModel_h_
#define __FormationModel_h_

#include "clients_kernel/Resolver.h"
#include "clients_kernel/ElementObserver_ABC.h"

namespace kernel
{
    class Controllers;
    class Entity_ABC;
    class Formation_ABC;
    class Team_ABC;
}

namespace xml
{ 
    class xostream;
    class xistream;
}

class FormationFactory_ABC;
class FormationLevels;
class Model;

// =============================================================================
/** @class  FormationModel
    @brief  FormationModel
*/
// Created: SBO 2006-09-19
// =============================================================================
class FormationModel : public kernel::Resolver< kernel::Formation_ABC >
                     , public kernel::Observer_ABC
                     , public kernel::ElementObserver_ABC< kernel::Formation_ABC >
{

public:
    //! @name Constructors/Destructor
    //@{
             FormationModel( kernel::Controllers& controllers, FormationFactory_ABC& formationFactory );
    virtual ~FormationModel();
    //@}

    //! @name Operations
    //@{
    void Create( kernel::Entity_ABC& parent, unsigned int levelId );
    void Create( xml::xistream& xis, kernel::Entity_ABC& parent, Model& model );
    void Purge();
    //@}

private:
    //! @name Copy/Assignement
    //@{
    FormationModel( const FormationModel& );            //!< Copy constructor
    FormationModel& operator=( const FormationModel& ); //!< Assignement operator
    //@}

    //! @name Helpers
    //@{
    virtual void NotifyDeleted( const kernel::Formation_ABC& formation );
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controllers& controllers_;
    FormationFactory_ABC& factory_;
    //@}

public:
    //! @name Member data
    //@{
    FormationLevels& levels_;
    //@}
};

#endif // __FormationModel_h_
