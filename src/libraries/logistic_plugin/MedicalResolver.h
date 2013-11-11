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
#include "ConsignResolver_ABC.h"


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

    //! @name Operations
    //@{
    virtual void WriteConsign( ConsignWriter& output ) const;
    virtual const ConsignData_ABC& ManageMessage( const ::sword::LogMedicalHandlingCreation& msg, ConsignResolver_ABC& resolver );
    virtual const ConsignData_ABC& ManageMessage( const ::sword::LogMedicalHandlingUpdate& msg, ConsignResolver_ABC& resolver );
    virtual const ConsignData_ABC& ManageMessage( const ::sword::LogMedicalHandlingDestruction& msg, ConsignResolver_ABC& resolver );
    //@}

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

// =============================================================================
/** @class  MedicalResolver
    @brief  MedicalResolver
*/
// Created: MMC 2012-08-06
// =============================================================================
class MedicalResolver : public ConsignResolver_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             MedicalResolver( const tools::Path& name, const NameResolver_ABC& nameResolver,
                    const std::string& header );
    virtual ~MedicalResolver();
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

#endif // __MedicalResolver_h_
