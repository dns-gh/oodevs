// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.

// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#ifndef plugins_hla_RprTransferSender_h
#define plugins_hla_RprTransferSender_h

#include "TransferSender_ABC.h"
#include "rpr/EntityIdentifier.h"
#include "dispatcher/Logger_ABC.h"
#include <hla/InteractionNotification_ABC.h>
#include <memory>
#include <map>

namespace dispatcher
{
    class Logger_ABC;
}

namespace plugins
{
namespace hla
{

class ContextFactory_ABC;
class InteractionBuilder;
template< typename Interaction > class InteractionSender_ABC;
class OwnershipStrategy_ABC;
class OwnershipController_ABC;

namespace interactions
{
    struct Acknowledge;
    struct TransferControl;
}

// =============================================================================
/** @class  RprTransferSender
    @brief  RprTransferSender
*/
// Created: AHC 2012-02-23
// =============================================================================
class RprTransferSender :  public TransferSender_ABC
                            , private ::hla::InteractionNotification_ABC< interactions::Acknowledge >
                            , private ::hla::InteractionNotification_ABC< interactions::TransferControl >
{
public:
    //! @name Constructors/Destructor
    //@{
    RprTransferSender(const rpr::EntityIdentifier federateID, const ContextFactory_ABC& ctxtFactory, const InteractionBuilder& builder,
            OwnershipStrategy_ABC& ownershipStrategy, OwnershipController_ABC& controller, dispatcher::Logger_ABC& logger );
    ~RprTransferSender();
    //@}

    //! @name Operations
    //@{
    virtual void RequestTransfer( const std::string& agentID, const TransferRequestCallback& callback, TransferType type, const std::vector< ::hla::AttributeIdentifier >& attributes, ::hla::VariableLengthData& tag );
    virtual void RequestTransfer( const std::vector< std::string >& agentIDs, const TransferRequestCallback& callback, TransferType type, const std::vector< ::hla::AttributeIdentifier >& attributes, ::hla::VariableLengthData& tag );
    //@}

private:
    //! @name Operations
    //@{
    virtual void Receive( interactions::Acknowledge& interaction );
    virtual void Receive( interactions::TransferControl& interaction );
    //@}

private:
    //! @name Types
    //@{
    typedef InteractionSender_ABC< interactions::Acknowledge > T_AcknowledgeSender;
    typedef InteractionSender_ABC< interactions::TransferControl > T_TransferSender;
    struct T_RequestInfo
    {
        T_RequestInfo( const std::string& n, TransferRequestCallback cb );
        std::string name_;
        TransferRequestCallback callback_;
    };
    typedef std::map< unsigned int, T_RequestInfo > T_Callbacks;
    //@}

private:
    //! @name Member data
    //@{
    const ContextFactory_ABC& ctxtFactory_;
    const rpr::EntityIdentifier federateID_;
    std::auto_ptr< T_AcknowledgeSender > pAcknowledgeSender_;
    std::auto_ptr< T_TransferSender > pTransferSender_;
    OwnershipStrategy_ABC& ownershipStrategy_;
    OwnershipController_ABC& ownershipController_;
    dispatcher::Logger_ABC& logger_;
    T_Callbacks callbacks_;
    //@}
};
}
}
#endif // plugins_hla_RprTransferSender_h
