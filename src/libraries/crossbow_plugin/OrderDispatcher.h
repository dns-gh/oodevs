// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __OrderDispatcher_h_
#define __OrderDispatcher_h_

#include "game_asn/Simulation.h"

namespace kernel
{
    class OrderType;
}

namespace dispatcher
{
    class SimulationPublisher_ABC;
    class Model;
    class Agent;
    class Automat;
}

namespace crossbow
{
    class Database_ABC;
    class OrderParameterSerializer;
    class Table_ABC;
    class Row_ABC;
    class OrderTypes;

// =============================================================================
/** @class  OrderDispatcher
    @brief  OrderDispatcher
*/
// Created: SBO 2007-05-31
// =============================================================================
class OrderDispatcher
{

public:
    //! @name Constructors/Destructor
    //@{
             OrderDispatcher( Database_ABC& database, const OrderTypes& types, const dispatcher::Model& model );
    virtual ~OrderDispatcher();
    //@}

    //! @name Operations
    //@{
    void Dispatch( dispatcher::SimulationPublisher_ABC& publisher, const Row_ABC& row );
    //@}

private:
    //! @name Copy/Assignment
    //@{
    OrderDispatcher( const OrderDispatcher& );            //!< Copy constructor
    OrderDispatcher& operator=( const OrderDispatcher& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    void DispatchMission( dispatcher::SimulationPublisher_ABC& publisher, const dispatcher::Agent& agent, const Row_ABC& row );
    void DispatchMission( dispatcher::SimulationPublisher_ABC& publisher, const dispatcher::Automat& automat, const Row_ABC& row );
    void DispatchFragOrder( dispatcher::SimulationPublisher_ABC& publisher, unsigned long targetId, const Row_ABC& row );

    unsigned long GetTargetId( const Row_ABC& row ) const;
    const kernel::OrderType* GetAgentMission( const Row_ABC& row ) const;
    const kernel::OrderType* GetAutomatMission( const Row_ABC& row ) const;
    void SetParameters( ASN1T_MissionParameters& parameters, unsigned long orderId, const kernel::OrderType& type );
    void SetParameter( ASN1T_MissionParameter& parameter, const Row_ABC& row, const kernel::OrderType& type );
    void CleanParameters( ASN1T_MissionParameters& parameters );
    //@}

private:
    //! @name Member data
    //@{    
    const OrderTypes& types_;
    const dispatcher::Model& model_;
//    Table_ABC& paramTable_;
    Database_ABC& database_;
    std::auto_ptr< OrderParameterSerializer > serializer_;
    //@}
};

}

#endif // __OrderDispatcher_h_
