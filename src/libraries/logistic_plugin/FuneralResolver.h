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

namespace sword
{
    class LogFuneralHandlingCreation;
    class LogFuneralHandlingUpdate;
}

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

private:
    virtual bool DoUpdateConsign( const sword::SimToClient& msg, const NameResolver_ABC& resolver );
    virtual void WriteConsign( ConsignWriter& output ) const;
    bool ManageMessage( const ::sword::LogFuneralHandlingCreation& msg,
            const NameResolver_ABC& resolver );
    bool ManageMessage( const ::sword::LogFuneralHandlingUpdate& msg,
            const NameResolver_ABC& resolver );

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

std::string GetFuneralHeader();

}
}

#endif // __FuneralResolver_h_
