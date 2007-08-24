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

    //! @name Types
    //@{
    typedef std::map< std::string, const OrderType* > T_OrderTypes;
    typedef T_OrderTypes::const_iterator            CIT_OrderTypes;
    //@}

    //! @name Helpers
    //@{
    void Load( const std::string& filename );
    void ReadOrderType( xml::xistream& xis, T_OrderTypes& resolver );
    //@}

private:
    //! @name Member data
    //@{
    T_OrderTypes unitMissions_;
    T_OrderTypes automatMissions_;
    T_OrderTypes fragOrders_;
    //@}
};

}

#endif // __OrderTypes_h_
