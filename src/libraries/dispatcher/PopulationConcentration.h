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

namespace MsgsSimToClient
{
    class MsgPopulationConcentrationCreation;
    class MsgPopulationConcentrationUpdate;
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
                              , public kernel::Updatable_ABC< MsgsSimToClient::MsgPopulationConcentrationUpdate >
{
public:
    //! @name Constructors/Destructor
    //@{
             PopulationConcentration( const Population& population, const MsgsSimToClient::MsgPopulationConcentrationCreation& msg );
    virtual ~PopulationConcentration();
    //@}

    //! @name Operations
    //@{
    virtual void DoUpdate       ( const MsgsSimToClient::MsgPopulationConcentrationUpdate& msg );
    virtual void SendCreation   ( ClientPublisher_ABC& publisher ) const;
    virtual void SendFullUpdate ( ClientPublisher_ABC& publisher ) const;
    virtual void SendDestruction( ClientPublisher_ABC& publisher ) const;
    virtual void Accept( kernel::ModelVisitor_ABC& visitor ) const;

    virtual unsigned int GetDeadHumans() const;
    virtual unsigned int GetLivingHumans() const;
    virtual unsigned int GetDensity() const;
    virtual QString      GetAttitude() const;

    virtual const Common::MsgCoordLatLong& GetPosition() const;
    //@}

private:
    //! @name Copy/Assignement
    //@{
    PopulationConcentration( const PopulationConcentration& );            //!< Copy constructor
    PopulationConcentration& operator=( const PopulationConcentration& ); //!< Assignement operator
    //@}

private:
    //! @name Member data
    //@{
    const Population&   population_;
    const unsigned long nID_;
    const Common::MsgCoordLatLong position_;

    unsigned long                nNbrAliveHumans_;
    unsigned long                nNbrDeadHumans_;
    Common::EnumPopulationAttitude nAttitude_;    
    //@}
};

}

#endif // __PopulationConcentration_h_
