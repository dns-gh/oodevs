// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __PopulationConcentration_h_
#define __PopulationConcentration_h_

#include "PopulationConcentration_ABC.h"
#include "protocol/SimulationSenders.h"

namespace sword
{
    class CrowdConcentrationCreation;
    class CrowdConcentrationUpdate;
}

namespace dispatcher
{
    class Population;

// =============================================================================
/** @class  PopulationConcentration
    @brief  PopulationConcentration
*/
// Created: NLD 2006-09-19
// =============================================================================
class PopulationConcentration : public dispatcher::PopulationConcentration_ABC
                              , public kernel::Extension_ABC
                              , public kernel::Updatable_ABC< sword::CrowdConcentrationUpdate >
{
public:
    //! @name Constructors/Destructor
    //@{
             PopulationConcentration( const Population& population, const sword::CrowdConcentrationCreation& msg );
    virtual ~PopulationConcentration();
    //@}

    //! @name Operations
    //@{
    virtual void DoUpdate       ( const sword::CrowdConcentrationUpdate& msg );
    virtual void SendCreation   ( ClientPublisher_ABC& publisher ) const;
    virtual void SendFullUpdate ( ClientPublisher_ABC& publisher ) const;
    virtual void SendDestruction( ClientPublisher_ABC& publisher ) const;
    virtual void Accept( kernel::ModelVisitor_ABC& visitor ) const;

    virtual unsigned int GetDeadHumans() const;
    virtual unsigned int GetLivingHumans() const;
    virtual float        GetDensity() const;
    virtual QString      GetAttitude() const;

    virtual const sword::CoordLatLong& GetPosition() const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    PopulationConcentration( const PopulationConcentration& );            //!< Copy constructor
    PopulationConcentration& operator=( const PopulationConcentration& ); //!< Assignment operator
    //@}

private:
    //! @name Member data
    //@{
    const Population&   population_;
    const unsigned long nID_;
    const sword::CoordLatLong position_;

    unsigned long             nNbrAliveHumans_;
    unsigned long             nNbrDeadHumans_;
    sword::EnumCrowdAttitude nAttitude_;
    //@}
};

}

#endif // __PopulationConcentration_h_
