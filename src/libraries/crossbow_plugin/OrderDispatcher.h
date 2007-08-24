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

#include "ESRI.h"
#include "game_asn/Asn.h"

namespace kernel
{
    class OrderTypes;
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
    class Connector;
    class OrderParameterSerializer;

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
             OrderDispatcher( Connector& connector, const kernel::OrderTypes& types, const dispatcher::Model& model );
    virtual ~OrderDispatcher();
    //@}

    //! @name Operations
    //@{
    void Dispatch( dispatcher::SimulationPublisher_ABC& publisher, const IRowPtr& row );
    //@}

private:
    //! @name Copy/Assignment
    //@{
    OrderDispatcher( const OrderDispatcher& );            //!< Copy constructor
    OrderDispatcher& operator=( const OrderDispatcher& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    void DispatchMission( dispatcher::SimulationPublisher_ABC& publisher, const dispatcher::Agent& agent, const IRowPtr& row );
    void DispatchMission( dispatcher::SimulationPublisher_ABC& publisher, const dispatcher::Automat& automat, const IRowPtr& row );
    void DispatchFragOrder( dispatcher::SimulationPublisher_ABC& publisher, unsigned long targetId, const IRowPtr& row );

    unsigned long GetTargetId( const IRowPtr& row ) const;
    const kernel::OrderType* GetAgentMission( const IRowPtr& row ) const;
    const kernel::OrderType* GetAutomatMission( const IRowPtr& row ) const;
    void SetParameters( ASN1T_MissionParameters& parameters, unsigned long orderId, const kernel::OrderType& type );
    void SetParameter( ASN1T_MissionParameter& parameter, const IRowPtr& row, const kernel::OrderType& type );
    void CleanParameters( ASN1T_MissionParameters& parameters );
    void SetOrderContext( ASN1T_OrderContext& asn, unsigned long orderId );
    void SetParameter( ASN1T_OrderContext& asn, const IRowPtr& row );
    void CleanOrderContext( ASN1T_OrderContext& asn );
    unsigned int GetLimaCount( unsigned long orderId );
    //@}

private:
    //! @name Member data
    //@{    
    const kernel::OrderTypes& types_;
    const dispatcher::Model& model_;
    ITablePtr paramTable_;
    std::auto_ptr< OrderParameterSerializer > serializer_;
    //@}
};

}

#endif // __OrderDispatcher_h_
