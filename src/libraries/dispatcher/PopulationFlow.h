 // *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __PopulationFlow_h_
#define __PopulationFlow_h_

#include "game_asn/Simulation.h"
#include "Localisation.h"
#include "SimpleEntity.h"
#include "clients_kernel/PopulationFlow_ABC.h"

namespace dispatcher
{
    class Population;
    class ModelVisitor_ABC;
    class ClientPublisher_ABC;

// =============================================================================
/** @class  PopulationFlow
    @brief  PopulationFlow
*/
// Created: NLD 2006-09-19
// =============================================================================
class PopulationFlow : public SimpleEntity< kernel::PopulationFlow_ABC >
{
public:
    //! @name Constructors/Destructor
    //@{
             PopulationFlow( const Population& population, const ASN1T_MsgPopulationFlowCreation& msg );
    virtual ~PopulationFlow();
    //@}

    //! @name Operations
    //@{
    void Update( const ASN1T_MsgPopulationFlowUpdate& msg );

    void SendCreation   ( ClientPublisher_ABC& publisher ) const;
    void SendFullUpdate ( ClientPublisher_ABC& publisher ) const;
    void SendDestruction( ClientPublisher_ABC& publisher ) const;
    void Accept( ModelVisitor_ABC& visitor ) const;

    virtual unsigned int GetDeadHumans() const;
    virtual unsigned int GetLivingHumans() const;
    virtual unsigned int GetDensity() const;
    virtual QString      GetAttitude() const;
    //@}

private:
    //! @name Copy/Assignement
    //@{
    PopulationFlow( const PopulationFlow& );            //!< Copy constructor
    PopulationFlow& operator=( const PopulationFlow& ); //!< Assignement operator
    //@}

private:
    const Population&   population_;
    const unsigned long nID_;

    Localisation                 path_;
    Localisation                 flow_;
    unsigned int                 nDirection_;
    unsigned int                 nSpeed_;
    unsigned long                nNbrAliveHumans_;
    unsigned long                nNbrDeadHumans_;
    ASN1T_EnumPopulationAttitude nAttitude_;    
};

}

#endif // __PopulationFlow_h_
