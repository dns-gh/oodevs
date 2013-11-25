// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#ifndef __MedicalResolver_h_
#define __MedicalResolver_h_

#include "ConsignData_ABC.h"

namespace sword
{
    class LogMedicalHandlingCreation;
    class LogMedicalHandlingUpdate;
    class LogMedicalHandlingDestruction;
}

namespace plugins
{
namespace logistic
{
// =============================================================================
/** @class  MedicalConsignData
    @brief  MedicalConsignData
*/
// Created: MMC 2012-08-06
// =============================================================================
class MedicalConsignData : public ConsignData_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             MedicalConsignData( const std::string& requestId )
                 : ConsignData_ABC( LogisticPlugin::eLogisticType_Medical, requestId ) {}
    virtual ~MedicalConsignData() {}
    //@}

private:
    virtual bool DoUpdateConsign( const sword::SimToClient& msg, const NameResolver_ABC& resolver );
    virtual void WriteConsign( ConsignWriter& output ) const;
    bool ManageMessage( const ::sword::LogMedicalHandlingCreation& msg,
            const NameResolver_ABC& resolver );
    bool ManageMessage( const ::sword::LogMedicalHandlingUpdate& msg,
            const NameResolver_ABC& resolver );
    bool ManageMessage( const ::sword::LogMedicalHandlingDestruction& msg,
            const NameResolver_ABC& resolver );

public:
    //! @name Member data
    //@{
    std::string creationTick_;
    std::string stateEndTick_;
    std::string unitId_;
    std::string providerId_;
    std::string nbc_;
    std::string mental_;
    std::string stateId_;
    std::string unit_;
    std::string provider_;
    std::string rank_;
    std::string wound_;
    std::string state_;
    //@}
};

std::string GetMedicalHeader();

}
}

#endif // __MedicalResolver_h_
