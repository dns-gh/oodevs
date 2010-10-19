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

#include "tools/Resolver.h"
#include "tools/ElementObserver_ABC.h"

namespace kernel
{
    class Controllers;
    class Entity_ABC;
    class Formation_ABC;
    class Team_ABC;
    class FormationLevels;
    class Automat_ABC;
}

namespace xml
{
    class xostream;
    class xistream;
}

class FormationFactory_ABC;
class Model;

// =============================================================================
/** @class  FormationModel
    @brief  FormationModel
*/
// Created: SBO 2006-09-19
// =============================================================================
class FormationModel : public tools::Resolver< kernel::Formation_ABC >
                     , public tools::Observer_ABC
                     , public tools::ElementObserver_ABC< kernel::Formation_ABC >
{
public:
    //! @name Constructors/Destructor
    //@{
    FormationModel( kernel::Controllers& controllers, FormationFactory_ABC& formationFactory, 
        const tools::Resolver< kernel::Automat_ABC>& automatResolver );
    virtual ~FormationModel();
    //@}

    //! @name Operations
    //@{
    kernel::Formation_ABC* Create( kernel::Entity_ABC& parent, unsigned int levelId, const QString& name = "" );
    void Create( xml::xistream& xis, kernel::Entity_ABC& parent, Model& model, std::string& loadingErrors );
    void Purge();
    //@}
    
    //! @name Delayed initialization
    //@{
    void ReadLogistic( xml::xistream& xis );
    template< typename H >
    void ReadLogisticLink( xml::xistream& xis, kernel::Formation_ABC& source, kernel::Entity_ABC& destination );
    //@}

private:
    //! @name Copy/Assignment
    //@{
    FormationModel( const FormationModel& );            //!< Copy constructor
    FormationModel& operator=( const FormationModel& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    virtual void NotifyDeleted( const kernel::Formation_ABC& formation );
    void ReadLogisticLink( xml::xistream& xis, kernel::Formation_ABC& automat );
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
    kernel::FormationLevels& levels_;
    const tools::Resolver< kernel::Automat_ABC>& automatResolver_;
    //@}
};

#endif // __FormationModel_h_
