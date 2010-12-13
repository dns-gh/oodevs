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

namespace sword
{
    class MsgMissionParameters;
    class MsgMissionParameter;
}

namespace kernel
{
    class OrderType;
}

namespace dispatcher
{
    class SimulationPublisher_ABC;
    class Model_ABC;
    class Agent_ABC;
    class Automat_ABC;
}

namespace plugins
{
namespace crossbow
{
    class Database_ABC;
    class Workspace_ABC;
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
             OrderDispatcher( Workspace_ABC& workspace, const OrderTypes& types, const dispatcher::Model_ABC& model );
    virtual ~OrderDispatcher();
    //@}

    //! @name Operations
    //@{
    void Dispatch( dispatcher::SimulationPublisher_ABC& publisher, const Row_ABC& row );
    bool IsValidOrder( const Row_ABC& row ) const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    OrderDispatcher( const OrderDispatcher& );            //!< Copy constructor
    OrderDispatcher& operator=( const OrderDispatcher& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    void Clean();
    //@}

    //! @name Mission Helpers
    //@{
    void DispatchAgentMission( dispatcher::SimulationPublisher_ABC& publisher, const dispatcher::Agent_ABC& agent, const Row_ABC& row );
    void DispatchAutomatMission( dispatcher::SimulationPublisher_ABC& publisher, const dispatcher::Automat_ABC& automat, const Row_ABC& row );
    void DispatchFragOrder( dispatcher::SimulationPublisher_ABC& publisher, unsigned long targetId, const Row_ABC& row );
    //@}

    //! @name Parameter Helpers
    //@{
    const kernel::OrderType* GetAgentMission( const Row_ABC& row ) const;
    const kernel::OrderType* GetAutomatMission( const Row_ABC& row ) const;
    void SetParameters( sword::MsgMissionParameters& parameters, unsigned long orderId, const kernel::OrderType& type );
    void SetParameter( sword::MsgMissionParameter& parameter, const Row_ABC& row, const kernel::OrderType& type );
    bool IsValidOrder( unsigned long orderId, const kernel::OrderType& type ) const;
    //@}

private:
    //! @name Member data
    //@{
    const OrderTypes& types_;
    const dispatcher::Model_ABC& model_;
    Workspace_ABC& workspace_;
    std::auto_ptr< OrderParameterSerializer > serializer_;
    //@}
};

}
}

#endif // __OrderDispatcher_h_
