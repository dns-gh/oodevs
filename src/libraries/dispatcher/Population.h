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
    class MsgCrowdOrder;
}

namespace MsgsSimToClient
{
    class MsgDecisionalState;
    class MsgCrowdFlowCreation;
    class MsgCrowdFlowUpdate;
    class MsgCrowdFlowDestruction;
    class MsgCrowdCreation;
    class MsgCrowdUpdate;
    class MsgCrowdConcentrationCreation;
    class MsgCrowdConcentrationUpdate;
    class MsgCrowdConcentrationDestruction;
}

namespace dispatcher
{
    class Model_ABC;
    class PopulationConcentration;
    class PopulationFlow;
    class PopulationOrder;
    class Team_ABC;

// =============================================================================
/** @class  Population
    @brief  Population
    // $$$$ SBO 2010-06-10: refactor Flow/Concentration resolver to avoid downcasting
*/
// Created: NLD 2006-09-19
// =============================================================================
class Population : public dispatcher::Population_ABC
                 , public kernel::Extension_ABC
                 , public kernel::Updatable_ABC< MsgsSimToClient::MsgCrowdCreation >
                 , public kernel::Updatable_ABC< MsgsSimToClient::MsgCrowdUpdate >
                 , public kernel::Updatable_ABC< MsgsSimToClient::MsgCrowdConcentrationCreation >
                 , public kernel::Updatable_ABC< MsgsSimToClient::MsgCrowdConcentrationUpdate >
                 , public kernel::Updatable_ABC< MsgsSimToClient::MsgCrowdConcentrationDestruction >
                 , public kernel::Updatable_ABC< MsgsSimToClient::MsgCrowdFlowCreation >
                 , public kernel::Updatable_ABC< MsgsSimToClient::MsgCrowdFlowUpdate >
                 , public kernel::Updatable_ABC< MsgsSimToClient::MsgCrowdFlowDestruction >
                 , public kernel::Updatable_ABC< Common::MsgCrowdOrder >
                 , public kernel::Updatable_ABC< MsgsSimToClient::MsgDecisionalState >
{
public:
    //! @name Constructors/Destructor
    //@{
             Population( Model_ABC& model, const MsgsSimToClient::MsgCrowdCreation& msg );
    virtual ~Population();
    //@}

    //! @name Operations
    //@{
    virtual void DoUpdate( const MsgsSimToClient::MsgCrowdCreation&                 msg );
    virtual void DoUpdate( const MsgsSimToClient::MsgCrowdUpdate&                   msg );
    virtual void DoUpdate( const MsgsSimToClient::MsgCrowdConcentrationCreation&    msg );
    virtual void DoUpdate( const MsgsSimToClient::MsgCrowdConcentrationUpdate&      msg );
    virtual void DoUpdate( const MsgsSimToClient::MsgCrowdConcentrationDestruction& msg );
    virtual void DoUpdate( const MsgsSimToClient::MsgCrowdFlowCreation&             msg );
    virtual void DoUpdate( const MsgsSimToClient::MsgCrowdFlowUpdate&               msg );
    virtual void DoUpdate( const MsgsSimToClient::MsgCrowdFlowDestruction&          msg );
    virtual void DoUpdate( const Common::MsgCrowdOrder&                             msg );
    virtual void DoUpdate( const MsgsSimToClient::MsgDecisionalState&               msg );

    virtual void SendCreation   ( ClientPublisher_ABC& publisher ) const;
    virtual void SendFullUpdate ( ClientPublisher_ABC& publisher ) const;
    virtual void SendDestruction( ClientPublisher_ABC& publisher ) const;

    virtual void Accept( kernel::ModelVisitor_ABC& visitor ) const;

    virtual const kernel::PopulationType& GetType() const;
    virtual unsigned int GetLivingHumans() const;
    virtual unsigned int GetDeadHumans() const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    Population( const Population& );            //!< Copy constructor
    Population& operator=( const Population& ); //!< Assignment operator
    //@}

private:
    //! @name Member data
    //@{
    Model_ABC& model_;
    const unsigned long nType_;
    const std::string strName_;
    dispatcher::Team_ABC& side_;
    unsigned int nDominationState_;
    std::auto_ptr< PopulationOrder > order_;
    DecisionalState decisionalInfos_;
    std::map< std::string, std::string > extensions_;
    //@}
};

}

#endif // __Population_h_
