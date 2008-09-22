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

#include "game_asn/Simulation.h"
#include "SimpleEntity.h"
#include "clients_kernel/PopulationConcentration_ABC.h"

namespace dispatcher
{
    class Population;
    class ModelVisitor_ABC;
    class ClientPublisher_ABC;

// =============================================================================
/** @class  PopulationConcentration
    @brief  PopulationConcentration
*/
// Created: NLD 2006-09-19
// =============================================================================
class PopulationConcentration : public SimpleEntity< kernel::PopulationConcentration_ABC >
{
public:
    //! @name Constructors/Destructor
    //@{
             PopulationConcentration( const Population& population, const ASN1T_MsgPopulationConcentrationCreation& msg );
    virtual ~PopulationConcentration();
    //@}

    //! @name Operations
    //@{
    void Update( const ASN1T_MsgPopulationConcentrationUpdate& msg );

    void SendCreation   ( ClientPublisher_ABC& publisher ) const;
    void SendFullUpdate ( ClientPublisher_ABC& publisher ) const;
    void SendDestruction( ClientPublisher_ABC& publisher ) const;

    virtual unsigned int GetDeadHumans() const;
    virtual unsigned int GetLivingHumans() const;
    virtual unsigned int GetDensity() const;
    virtual QString      GetAttitude() const;

    void Accept( ModelVisitor_ABC& visitor ) const;
    //@}

private:
    //! @name Copy/Assignement
    //@{
    PopulationConcentration( const PopulationConcentration& );            //!< Copy constructor
    PopulationConcentration& operator=( const PopulationConcentration& ); //!< Assignement operator
    //@}

private:
    const Population&   population_;
    const unsigned long nID_;
    const ASN1T_CoordLatLong position_;

    unsigned long                nNbrAliveHumans_;
    unsigned long                nNbrDeadHumans_;
    ASN1T_EnumPopulationAttitude nAttitude_;    
};

}

#endif // __PopulationConcentration_h_
