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

#include "astec_kernel/Entity_ABC.h"
#include "astec_kernel/Resolver.h"

class PopulationFlow;
class PopulationConcentration;
class Team;
class PopulationType;

// =============================================================================
/** @class  Population_ABC
    @brief  Population_ABC
*/
// Created: AGE 2006-08-07
// =============================================================================
class Population_ABC : public Entity_ABC
                     , public Resolver< PopulationFlow >
                     , public Resolver< PopulationConcentration >
{

public:
    //! @name Constructors/Destructor
    //@{
             Population_ABC();
    virtual ~Population_ABC();
    //@}

    //! @name Operations
    //@{
    virtual bool IsInTeam( const Team& team ) const = 0;
    virtual const Team& GetTeam() const = 0;
    virtual const PopulationType& GetType() const = 0;

    virtual unsigned int GetLivingHumans() const = 0;
    virtual unsigned int GetDeadHumans() const = 0;
    //@}

    //! @name Helpers
    //@{
    const PopulationFlow* FindFlow( unsigned long id ) const {
        return Resolver< PopulationFlow >::Find( id );
    }
    const PopulationFlow& GetFlow( unsigned long id ) const {
        return Resolver< PopulationFlow >::Get( id );
    }
    const PopulationConcentration* FindConcentration( unsigned long id ) const {
        return Resolver< PopulationConcentration >::Find( id );
    }
    const PopulationConcentration& GetConcentration( unsigned long id ) const {
        return Resolver< PopulationConcentration >::Get( id );
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

    //! @name Helpers
    //@{
    //@}

private:
    //! @name Member data
    //@{
    //@}
};

#endif // __Population_ABC_h_
