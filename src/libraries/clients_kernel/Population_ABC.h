// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __Population_ABC_h_
#define __Population_ABC_h_

#include "Entity_ABC.h"
#include "Resolver.h"

namespace kernel
{
    class Team_ABC;
    class PopulationType;
    class PopulationFlow_ABC;
    class PopulationConcentration_ABC;

// =============================================================================
/** @class  Population_ABC
    @brief  Population_ABC
*/
// Created: AGE 2006-08-07
// =============================================================================
class Population_ABC : public Entity_ABC
                     , public Resolver< PopulationFlow_ABC >
                     , public Resolver< PopulationConcentration_ABC >
{

public:
    //! @name Constructors/Destructor
    //@{
             Population_ABC();
    virtual ~Population_ABC();
    //@}

    //! @name Operations
    //@{
    virtual const PopulationType& GetType() const = 0;

    virtual unsigned int GetLivingHumans() const = 0;
    virtual unsigned int GetDeadHumans() const = 0;
    //@}

    //! @name Helpers
    //@{
    const PopulationFlow_ABC* FindFlow( unsigned long id ) const {
        return Resolver< PopulationFlow_ABC >::Find( id );
    }
    const PopulationFlow_ABC& GetFlow( unsigned long id ) const {
        return Resolver< PopulationFlow_ABC >::Get( id );
    }
    const PopulationConcentration_ABC* FindConcentration( unsigned long id ) const {
        return Resolver< PopulationConcentration_ABC >::Find( id );
    }
    const PopulationConcentration_ABC& GetConcentration( unsigned long id ) const {
        return Resolver< PopulationConcentration_ABC >::Get( id );
    }
    //@}

    //! @name Operations
    //@{
    virtual void Select( ActionController& controller ) const;
    virtual void ContextMenu( ActionController& controller, const QPoint& where ) const;
    virtual void Activate( ActionController& controller ) const;
    //@}

private:
    //! @name Copy/Assignement
    //@{
    Population_ABC( const Population_ABC& );            //!< Copy constructor
    Population_ABC& operator=( const Population_ABC& ); //!< Assignement operator
    //@}
};

}

#endif // __Population_ABC_h_
