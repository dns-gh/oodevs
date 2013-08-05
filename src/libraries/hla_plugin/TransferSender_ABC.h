// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#ifndef plugins_hla_TransferSender_ABC_h
#define plugins_hla_TransferSender_ABC_h

#include <boost/function.hpp>
#include <string>
#include <vector>

namespace hla
{
    class AttributeIdentifier;
}

namespace plugins
{
namespace hla
{

// =============================================================================
/** @class  TransferSender_ABC
    @brief  TransferSender_ABC
*/
// Created: AHC 2012-02-23
// =============================================================================
class TransferSender_ABC
{
public:
    enum TransferType
    {
        E_EntityPush  = 1,
        E_EntityPull  = 2,
    };

    typedef boost::function< void ( bool ) > TransferRequestCallback;

    //! @name Constructors/Destructor
    //@{
    virtual ~TransferSender_ABC() {}
    //@}

    //! @name Operations
    //@{
    virtual void RequestTransfer( const std::string& agentID, const TransferRequestCallback& callback, TransferType type, const std::vector< ::hla::AttributeIdentifier >& attributes ) = 0;
    virtual void RequestTransfer( const std::vector< std::string >& agentIDs, const TransferRequestCallback& callback, TransferType type, const std::vector< ::hla::AttributeIdentifier >& attributes ) = 0;
    //@}
};

}
}
#endif // plugins_hla_TransferSender_ABC_h
