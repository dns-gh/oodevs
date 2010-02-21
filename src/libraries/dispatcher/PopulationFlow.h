 // *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#ifndef __PopulationFlow_h_
#define __PopulationFlow_h_


#include "Localisation.h"
#include "SimpleEntity.h"
#include "clients_kernel/PopulationFlow_ABC.h"

namespace kernel
{
    class ModelVisitor_ABC;
}
namespace Common
{
    enum EnumPopulationAttitude;
}

namespace MsgsSimToClient
{
    class MsgPopulationFlowCreation;
    class MsgPopulationFlowUpdate;
}

namespace dispatcher
{
    class Population;
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
             PopulationFlow( const Population& population, const MsgsSimToClient::MsgPopulationFlowCreation& msg );
    virtual ~PopulationFlow();
    //@}

    //! @name Operations
    //@{
    void Update( const MsgsSimToClient::MsgPopulationFlowUpdate& msg );

    void SendCreation   ( ClientPublisher_ABC& publisher ) const;
    void SendFullUpdate ( ClientPublisher_ABC& publisher ) const;
    void SendDestruction( ClientPublisher_ABC& publisher ) const;
    void Accept( kernel::ModelVisitor_ABC& visitor ) const;

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

public:
    const Population&   population_;
    const unsigned long nID_;

    Localisation                 path_;
    Localisation                 flow_;
    unsigned int                 nDirection_;
    unsigned int                 nSpeed_;
    unsigned long                nNbrAliveHumans_;
    unsigned long                nNbrDeadHumans_;
    Common::EnumPopulationAttitude nAttitude_;    
};

}

#endif // __PopulationFlow_h_
