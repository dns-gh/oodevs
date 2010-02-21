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

#include "SimpleEntity.h"
#include "clients_kernel/Population_ABC.h"
#include "DecisionalState.h"

namespace Common
{
    class MsgPopulationOrder;
}

namespace MsgsSimToClient
{
    class MsgDecisionalState;
    class MsgPopulationFlowCreation;
    class MsgPopulationFlowUpdate;
    class MsgPopulationFlowDestruction;
    class MsgPopulationCreation;
    class MsgPopulationUpdate;
    class MsgPopulationConcentrationCreation;
    class MsgPopulationConcentrationUpdate;  
    class MsgPopulationConcentrationDestruction;
}

namespace kernel
{
    class ModelVisitor_ABC;
    class Team_ABC;
}

namespace dispatcher
{
    class Model;
    class PopulationConcentration;
    class PopulationFlow;
    class PopulationOrder;

// =============================================================================
/** @class  Population
    @brief  Population
*/
// Created: NLD 2006-09-19
// =============================================================================
class Population : public SimpleEntity< kernel::Population_ABC >
{
public:
    //! @name Constructors/Destructor
    //@{
             Population( Model& model, const MsgsSimToClient::MsgPopulationCreation& msg );
    virtual ~Population();
    //@}

    //! @name Operations
    //@{
    void Update( const MsgsSimToClient::MsgPopulationCreation&                 msg );
    void Update( const MsgsSimToClient::MsgPopulationUpdate&                   msg );
    void Update( const MsgsSimToClient::MsgPopulationConcentrationCreation&    msg );
    void Update( const MsgsSimToClient::MsgPopulationConcentrationUpdate&      msg );
    void Update( const MsgsSimToClient::MsgPopulationConcentrationDestruction& msg );
    void Update( const MsgsSimToClient::MsgPopulationFlowCreation&             msg );
    void Update( const MsgsSimToClient::MsgPopulationFlowUpdate&               msg );
    void Update( const MsgsSimToClient::MsgPopulationFlowDestruction&          msg );
    void Update( const Common::MsgPopulationOrder&                    msg );
    void Update( const MsgsSimToClient::MsgDecisionalState&                    msg );

    void SendCreation   ( ClientPublisher_ABC& publisher ) const;
    void SendFullUpdate ( ClientPublisher_ABC& publisher ) const;
    void SendDestruction( ClientPublisher_ABC& publisher ) const;

    void Accept( kernel::ModelVisitor_ABC& visitor ) const;

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

private:
    //! @name Member data
    //@{
          Model&        model_;
    const unsigned long nType_;
    const std::string   strName_;
    kernel::Team_ABC&   side_;
    
    unsigned int        nDominationState_;
    std::auto_ptr< PopulationOrder > order_;
    DecisionalState decisionalInfos_;
    //@}

public:
    tools::Resolver< PopulationConcentration > concentrations_;
    tools::Resolver< PopulationFlow          > flows_;

};

}

#endif // __Population_h_
