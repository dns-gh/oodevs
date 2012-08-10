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
             MedicalConsignData();
    virtual ~MedicalConsignData() {}
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
    int providerId_;
    int nbc_;
    int mental_;
    int stateId_;
    std::string simTime_;
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
             MedicalResolver( const std::string& name, const dispatcher::Model_ABC& model );
    virtual ~MedicalResolver();
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

#endif // __MedicalResolver_h_