 // *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __LogConsignSupply_h_
#define __LogConsignSupply_h_


#include "SimpleEntity.h"
#include "tools/Resolver.h"

namespace sword
{
    class ParentEntity;
}

namespace sword
{
    enum EnumLogSupplyHandlingStatus;
    class LogSupplyHandlingCreation;
    class LogSupplyHandlingUpdate;
}

namespace kernel
{
    class Automat_ABC;
    class Entity_ABC;
    class Agent_ABC;
    class ModelVisitor_ABC;
}

namespace dispatcher
{
    class Agent;
    class Automat;
    class Model;
    class LogSupplyDotation;
    class ClientPublisher_ABC;

// =============================================================================
/** @class  LogConsignSupply
    @brief  LogConsignSupply
*/
// Created: NLD 2006-09-19
// =============================================================================
class LogConsignSupply : public SimpleEntity< >
{
public:
    //! @name Constructors/Destructor
    //@{F
             LogConsignSupply( const Model& model, const sword::LogSupplyHandlingCreation& msg );
    virtual ~LogConsignSupply();
    //@}

    //! @name Operations
    //@{
    using kernel::Entity_ABC::Update;
    void Update( const sword::LogSupplyHandlingUpdate& msg );
    void SendCreation   ( ClientPublisher_ABC& publisher ) const;
    void SendFullUpdate ( ClientPublisher_ABC& publisher ) const;
    void SendDestruction( ClientPublisher_ABC& publisher ) const;
    void Accept( kernel::ModelVisitor_ABC& visitor ) const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    LogConsignSupply( const LogConsignSupply& );            //!< Copy constructor
    LogConsignSupply& operator=( const LogConsignSupply& ); //!< Assignment operator
    //@}
    //! @name Tools
	//@{
	kernel::Entity_ABC* FindLogEntity(const sword::ParentEntity& msg) const;
	void FillLogEntityID(sword::ParentEntity& msg, const kernel::Entity_ABC* entity) const;
	//@}

private:
    const Model&               model_;
    const kernel::Automat_ABC& automat_;
    const unsigned long        nTickCreation_;

    const kernel::Entity_ABC*        pTreatingEntity_;
    const kernel::Entity_ABC*        pConvoyingEntity_;
    const kernel::Agent_ABC*          pConvoy_;
    sword::EnumLogSupplyHandlingStatus nState_;
    tools::Resolver< LogSupplyDotation > dotations_;
};

}

#endif // __LogConsignSupply_h_
