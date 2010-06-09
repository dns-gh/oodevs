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

#include "Population_ABC.h"
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
}

namespace dispatcher
{
    class Model;
    class PopulationConcentration;
    class PopulationFlow;
    class PopulationOrder;
    class Team_ABC;

// =============================================================================
/** @class  Population
    @brief  Population
*/
// Created: NLD 2006-09-19
// =============================================================================
class Population : public dispatcher::Population_ABC
                 , public kernel::Extension_ABC
                 , public kernel::Updatable_ABC< MsgsSimToClient::MsgPopulationCreation >
                 , public kernel::Updatable_ABC< MsgsSimToClient::MsgPopulationUpdate >
                 , public kernel::Updatable_ABC< MsgsSimToClient::MsgPopulationConcentrationCreation >
                 , public kernel::Updatable_ABC< MsgsSimToClient::MsgPopulationConcentrationUpdate >
                 , public kernel::Updatable_ABC< MsgsSimToClient::MsgPopulationConcentrationDestruction >
                 , public kernel::Updatable_ABC< MsgsSimToClient::MsgPopulationFlowCreation >
                 , public kernel::Updatable_ABC< MsgsSimToClient::MsgPopulationFlowUpdate >
                 , public kernel::Updatable_ABC< MsgsSimToClient::MsgPopulationFlowDestruction >
                 , public kernel::Updatable_ABC< Common::MsgPopulationOrder >
                 , public kernel::Updatable_ABC< MsgsSimToClient::MsgDecisionalState >
{
public:
    //! @name Constructors/Destructor
    //@{
             Population( Model& model, const MsgsSimToClient::MsgPopulationCreation& msg );
    virtual ~Population();
    //@}

    //! @name Operations
    //@{
    virtual void DoUpdate( const MsgsSimToClient::MsgPopulationCreation&                 msg );
    virtual void DoUpdate( const MsgsSimToClient::MsgPopulationUpdate&                   msg );
    virtual void DoUpdate( const MsgsSimToClient::MsgPopulationConcentrationCreation&    msg );
    virtual void DoUpdate( const MsgsSimToClient::MsgPopulationConcentrationUpdate&      msg );
    virtual void DoUpdate( const MsgsSimToClient::MsgPopulationConcentrationDestruction& msg );
    virtual void DoUpdate( const MsgsSimToClient::MsgPopulationFlowCreation&             msg );
    virtual void DoUpdate( const MsgsSimToClient::MsgPopulationFlowUpdate&               msg );
    virtual void DoUpdate( const MsgsSimToClient::MsgPopulationFlowDestruction&          msg );
    virtual void DoUpdate( const Common::MsgPopulationOrder&                    msg );
    virtual void DoUpdate( const MsgsSimToClient::MsgDecisionalState&                    msg );

    virtual void SendCreation   ( ClientPublisher_ABC& publisher ) const;
    virtual void SendFullUpdate ( ClientPublisher_ABC& publisher ) const;
    virtual void SendDestruction( ClientPublisher_ABC& publisher ) const;

    virtual void Accept( kernel::ModelVisitor_ABC& visitor ) const;

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
    const std::string strName_;
    dispatcher::Team_ABC& side_;
    
    unsigned int nDominationState_;
    std::auto_ptr< PopulationOrder > order_;
    DecisionalState decisionalInfos_;

    tools::Resolver< PopulationConcentration > concentrations_;
    tools::Resolver< PopulationFlow > flows_;
    //@}
};

}

#endif // __Population_h_
