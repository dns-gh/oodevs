// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#ifndef __OrderDispatcher_h_
#define __OrderDispatcher_h_

namespace Common
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
    class Model;
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
    @brief  Order dispatcher
*/
// Created: SBO 2007-05-31
// =============================================================================
class OrderDispatcher
{
public:
    //! @name Constructors/Destructor
    //@{
             OrderDispatcher( Workspace_ABC& workspace, const OrderTypes& types, const dispatcher::Model& model );
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
    void Clean();
    //@}

    //! @name Mission Helpers
    //@{
    void DispatchMission( dispatcher::SimulationPublisher_ABC& publisher, const dispatcher::Agent_ABC& agent, const Row_ABC& row );
    void DispatchMission( dispatcher::SimulationPublisher_ABC& publisher, const dispatcher::Automat_ABC& automat, const Row_ABC& row );
    void DispatchFragOrder( dispatcher::SimulationPublisher_ABC& publisher, unsigned long targetId, const Row_ABC& row );
    //@}

    //! @name Parameter Helpers
    //@{
    unsigned long GetTargetId( const Row_ABC& row ) const;
    const kernel::OrderType* GetAgentMission( const Row_ABC& row ) const;
    const kernel::OrderType* GetAutomatMission( const Row_ABC& row ) const;
    void SetParameters( Common::MsgMissionParameters& parameters, unsigned long orderId, const kernel::OrderType& type );
    void SetParameter( Common::MsgMissionParameter& parameter, const Row_ABC& row, const kernel::OrderType& type );
    //@}

private:
    //! @name Member data
    //@{
    const OrderTypes& types_;
    const dispatcher::Model& model_;
    Database_ABC& database_;
    std::auto_ptr< OrderParameterSerializer > serializer_;
    //@}
};

}
}

#endif // __OrderDispatcher_h_
