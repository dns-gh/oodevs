// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __OrderType_h_
#define __OrderType_h_

#include "ENT/ENT_Enums.h"
#include <tools/Resolver.h>

namespace xml
{
    class xistream;
}

namespace kernel
{
    class OrderParameter;

// =============================================================================
/** @class  OrderType
    @brief  OrderType
*/
// Created: SBO 2007-04-23
// =============================================================================
class OrderType : public tools::Resolver< OrderParameter >
{
public:
    //! @name Constructors/Destructor
    //@{
             OrderType();
             OrderType( const std::string& name, unsigned long id );
    explicit OrderType( xml::xistream& xis );
    virtual ~OrderType();
    //@}

    //! @name Operations
    //@{
    unsigned long GetId() const;
    const std::string& GetName() const;
    const std::string& GetDoctrineInformation() const;
    const std::string& GetUsageInformation() const;
    virtual E_MissionType GetType() const;
    //@}

private:
    //! @name Member data
    //@{
    std::string name_;
    unsigned long id_;
    std::string doctrine_;
    std::string usage_;
    //@}
};

}

#endif // __OrderType_h_
