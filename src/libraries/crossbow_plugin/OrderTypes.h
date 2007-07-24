// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __OrderTypes_h_
#define __OrderTypes_h_

#include "Resolver.h"

namespace xml
{
    class xistream;
}

namespace dispatcher
{
    class Config;
}

namespace kernel
{
    class OrderType;

// =============================================================================
/** @class  OrderTypes
    @brief  OrderTypes
*/
// Created: SBO 2007-05-31
// =============================================================================
class OrderTypes
{

public:
    //! @name Constructors/Destructor
    //@{
    explicit OrderTypes( const dispatcher::Config& config );
    virtual ~OrderTypes();
    //@}

    //! @name Operations
    //@{
    const OrderType* FindAgentMission  ( const std::string& name ) const;
    const OrderType* FindAutomatMission( const std::string& name ) const;
    const OrderType* FindFragOrder     ( const std::string& name ) const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    OrderTypes( const OrderTypes& );            //!< Copy constructor
    OrderTypes& operator=( const OrderTypes& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    void Load( const std::string& filename );
    void ReadOrderType( xml::xistream& xis, Resolver< OrderType, std::string >& resolver );
    //@}

private:
    //! @name Member data
    //@{
    Resolver< OrderType, std::string > unitMissions_;
    Resolver< OrderType, std::string > automatMissions_;
    Resolver< OrderType, std::string > fragOrders_;
    //@}
};

}

#endif // __OrderTypes_h_
