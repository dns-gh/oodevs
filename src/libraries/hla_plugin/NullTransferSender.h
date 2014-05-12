// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#ifndef plugins_hla_NullTransferSender_ABC_h
#define plugins_hla_NullTransferSender_ABC_h

#include "TransferSender_ABC.h"
#include "rpr/EntityIdentifier.h"
#include <hla/InteractionNotification_ABC.h>
#include <memory>

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
/** @class  NullTransferSender
    @brief  NullTransferSender
*/
// Created: AHC 2012-02-23
// =============================================================================
class NullTransferSender : public TransferSender_ABC
                         , private ::hla::InteractionNotification_ABC< interactions::Acknowledge >
                         , private ::hla::InteractionNotification_ABC< interactions::TransferControl >
{
public:
    NullTransferSender( const rpr::EntityIdentifier federateID, const ContextFactory_ABC& ctxtFactory, const InteractionBuilder& builder,
            OwnershipStrategy_ABC& ownershipStrategy, OwnershipController_ABC& controller, dispatcher::Logger_ABC& logger );
    virtual ~NullTransferSender();

    //! @name Operations
    //@{
    virtual void RequestTransfer( const std::string& agentID, const TransferRequestCallback& callback, TransferType type, const std::vector< ::hla::AttributeIdentifier >& attributes, ::hla::VariableLengthData& tag, uint32_t capability = 0 );
    virtual void RequestTransfer( const std::vector< std::string >& agentIDs, const TransferRequestCallback& callback, TransferType type, const std::vector< ::hla::AttributeIdentifier >& attributes, ::hla::VariableLengthData& tag, uint32_t capability = 0 );
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
    //@}

private:
    //! @name Member data
    //@{
    const ContextFactory_ABC& ctxtFactory_;
    const rpr::EntityIdentifier federateID_;
    std::unique_ptr< T_AcknowledgeSender > pAcknowledgeSender_;
    std::unique_ptr< T_TransferSender > pTransferSender_;
    OwnershipStrategy_ABC& ownershipStrategy_;
    OwnershipController_ABC& ownershipController_;
};

}
}
#endif // plugins_hla_NullTransferSender_ABC_h
