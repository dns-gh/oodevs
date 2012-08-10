
// SupplyResolver.h

// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#ifndef __SupplyResolver_h_
#define __SupplyResolver_h_

#include "ConsignData_ABC.h"
#include "ConsignResolver_ABC.h"


namespace plugins
{
namespace logistic
{
// =============================================================================
/** @class  SupplyConsignData
    @brief  SupplyConsignData
*/
// Created: MMC 2012-08-06
// =============================================================================
class SupplyConsignData : public ConsignData_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
        SupplyConsignData();
        virtual ~SupplyConsignData() {}
    //@}

    //! @name Operations
    //@{
    virtual void Write( std::ofstream& output );
    //@}

public:

    //! @name Types
    //@{
    struct Resource
    {
        Resource() : id_( 0 ), requested_( 0 ), granted_( 0 ), conveyed_( 0 ) {}
        virtual ~Resource() {}

        std::string type_;
        int id_;
        int requested_;
        int granted_;
        int conveyed_;
    };
    //@}

public:
    //! @name Member data
    //@{
    int tick_;
    int automatId_;
    int providerId_;
    int transportProviderId_;
    int conveyorId_;
    int stateId_;
    std::string simTime_;
    std::string automat_;
    std::string provider_;
    std::string transportProvider_;
    std::string conveyor_;
    std::string state_;
    std::vector< Resource > resources_;
    //@}
};

// =============================================================================
/** @class  SupplyResolver
    @brief  SupplyResolver
*/
// Created: MMC 2012-08-06
// =============================================================================
class SupplyResolver : public ConsignResolver_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             SupplyResolver( const std::string& name, const dispatcher::Model_ABC& model );
    virtual ~SupplyResolver();
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

#endif // __SupplyResolver_h_