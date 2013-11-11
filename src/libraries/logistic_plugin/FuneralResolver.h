// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#ifndef __FuneralResolver_h_
#define __FuneralResolver_h_

#include "ConsignData_ABC.h"
#include "ConsignResolver_ABC.h"

namespace plugins
{
namespace logistic
{
// =============================================================================
/** @class  FuneralConsignData
    @brief  FuneralConsignData
*/
// Created: MMC 2012-08-06
// =============================================================================
class FuneralConsignData : public ConsignData_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             FuneralConsignData( const std::string& requestId )
                 : ConsignData_ABC( LogisticPlugin::eLogisticType_Funeral, requestId ) {}
    virtual ~FuneralConsignData() {}
    //@}

    //! @name Operations
    //@{
    virtual void WriteConsign( ConsignWriter& output ) const;
    virtual const ConsignData_ABC& ManageMessage( const ::sword::LogFuneralHandlingCreation& msg, ConsignResolver_ABC& resolver );
    virtual const ConsignData_ABC& ManageMessage( const ::sword::LogFuneralHandlingUpdate& msg, ConsignResolver_ABC& resolver );
    //@}

public:
    //! @name Member data
    //@{
    std::string creationTick_;
    std::string stateEndTick_;
    std::string unitId_;
    std::string handlingUnitId_;
    std::string conveyingUnitId_;
    std::string packagingResourceId_;
    std::string stateId_;
    std::string unit_;
    std::string handlingUnit_;
    std::string conveyingUnit_;
    std::string rank_;
    std::string packagingResource_;
    std::string state_;
    //@}
};

// =============================================================================
/** @class  FuneralResolver
    @brief  FuneralResolver
*/
// Created: MMC 2012-08-06
// =============================================================================
class FuneralResolver : public ConsignResolver_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             FuneralResolver( const tools::Path& name, const NameResolver_ABC& nameResolver );
    virtual ~FuneralResolver();
    //@}

    //! @name Operations
    //@{
    virtual void InitHeader();
    //@}

protected:
    //! @name Operations
    //@{
    virtual boost::optional< std::string > ManageMessage(
            const sword::SimToClient& message, ConsignData_ABC& consign );
    //@}
};
}
}

#endif // __FuneralResolver_h_
