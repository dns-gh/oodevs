// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __Population_h_
#define __Population_h_

#include "clients_kernel/EntityImplementation.h"
#include "clients_kernel/Population_ABC.h"
#include "clients_kernel/Resolver_ABC.h"
#include "clients_kernel/Serializable_ABC.h"
#include "ENT/ENT_Tr.h"

namespace kernel
{
    class Team_ABC;
    class Controller;
    class PopulationType;
}

namespace xml
{
    class xistream;
}

class IdManager;

// =============================================================================
/** @class  Population
    @brief  Population
*/
// Created: SBO 2006-11-08
// =============================================================================
class Population : public kernel::EntityImplementation< kernel::Population_ABC >
                 , public kernel::Extension_ABC
                 , public kernel::Serializable_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             Population( const kernel::PopulationType& type, kernel::Controller& controller, IdManager& idManager );
             Population( xml::xistream& xis, kernel::Controller& controller, IdManager& idManager, const kernel::Resolver_ABC< kernel::PopulationType, QString >& types );
    virtual ~Population();
    //@}

    //! @name Operations
    //@{
    virtual const kernel::Team_ABC& GetTeam() const;
    virtual const kernel::PopulationType& GetType() const;

    virtual unsigned int GetLivingHumans() const;
    virtual unsigned int GetDeadHumans() const;
    //@}

private:
    //! @name Copy/Assignement
    //@{
    Population( const Population& );            //!< Copy constructor
    Population& operator=( const Population& ); //!< Assignement operator
    //@}

    //! @name Helpers
    //@{
    void CreateDictionary( kernel::Controller& controller );
    virtual void SerializeAttributes( xml::xostream& xos ) const;
    //@}

private:
    //! @name Member data
    //@{
    const kernel::PopulationType& type_;
    unsigned long                 livingHumans_;
    E_PopulationAttitude          attitude_;
    //@}
};

#endif // __Population_h_
