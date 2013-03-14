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
             MedicalConsignData( const std::string& requestId ) : ConsignData_ABC( requestId ) {}
    virtual ~MedicalConsignData() {}
    //@}

    //! @name Operations
    //@{
    virtual void operator>>( std::stringstream& output ) const;
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
             MedicalResolver( const tools::Path& name, const NameResolver_ABC& nameResolver );
    virtual ~MedicalResolver();
    //@}

    //! @name Operations
    //@{
    virtual void InitHeader();
    //@}

protected:
    //! @name Operations
    //@{
    virtual bool IsManageable( const sword::SimToClient& message );
    virtual bool IsEmptyLineMessage( const sword::SimToClient& message );
    virtual void ManageMessage( const sword::SimToClient& message );
    virtual ConsignData_ABC* CreateConsignData( int requestId );
    //@}
};
}
}

#endif // __MedicalResolver_h_