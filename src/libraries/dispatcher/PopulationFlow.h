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

#include "Localisation.h"
#include "PopulationFlow_ABC.h"

namespace Common
{
    enum EnumCrowdAttitude;
}

namespace MsgsSimToClient
{
    class MsgCrowdFlowCreation;
    class MsgCrowdFlowUpdate;
}

namespace dispatcher
{
    class Population;

// =============================================================================
/** @class  PopulationFlow
    @brief  PopulationFlow
*/
// Created: NLD 2006-09-19
// =============================================================================
class PopulationFlow : public dispatcher::PopulationFlow_ABC
                     , public kernel::Extension_ABC
                     , public kernel::Updatable_ABC< MsgsSimToClient::MsgCrowdFlowUpdate >
{
public:
    //! @name Constructors/Destructor
    //@{
             PopulationFlow( const Population& population, const MsgsSimToClient::MsgCrowdFlowCreation& msg );
    virtual ~PopulationFlow();
    //@}

    //! @name Operations
    //@{
    virtual void DoUpdate( const MsgsSimToClient::MsgCrowdFlowUpdate& msg );

    virtual void SendCreation   ( ClientPublisher_ABC& publisher ) const;
    virtual void SendFullUpdate ( ClientPublisher_ABC& publisher ) const;
    virtual void SendDestruction( ClientPublisher_ABC& publisher ) const;
    virtual void Accept( kernel::ModelVisitor_ABC& visitor ) const;

    virtual unsigned int GetDeadHumans() const;
    virtual unsigned int GetLivingHumans() const;
    virtual float        GetDensity() const;
    virtual QString      GetAttitude() const;

    const Localisation& GetLocation() const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    PopulationFlow( const PopulationFlow& );            //!< Copy constructor
    PopulationFlow& operator=( const PopulationFlow& ); //!< Assignment operator
    //@}

private:
    //! @name Member data
    //@{
    const Population&   population_;
    const unsigned long nID_;

    Localisation                 path_;
    Localisation                 flow_;
    unsigned int                 nDirection_;
    unsigned int                 nSpeed_;
    unsigned long                nNbrAliveHumans_;
    unsigned long                nNbrDeadHumans_;
    Common::EnumCrowdAttitude    nAttitude_;
    //@}
};

}

#endif // __PopulationFlow_h_
