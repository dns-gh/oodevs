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
             FuneralConsignData();
    virtual ~FuneralConsignData() {}
    //@}

    //! @name Operations
    //@{
    virtual void Write( std::ofstream& output );
    //@}

public:
    //! @name Member data
    //@{
    int tick_;
    int unitId_;
    int handlingUnitId_;
    int conveyingUnitId_;
    int packagingResourceId_;
    int stateId_;
    std::string simTime_;
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
             FuneralResolver( const std::string& name, const dispatcher::Model_ABC& model );
    virtual ~FuneralResolver();
    //@}

protected:
    //! @name Operations
    //@{
    virtual bool IsManageable( const sword::SimToClient& message );
    virtual void ManageMessage( const sword::SimToClient& message );
    //@}
};
}
}

#endif // __FuneralResolver_h_