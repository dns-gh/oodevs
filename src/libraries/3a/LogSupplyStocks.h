// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef __LogSupplyStocks_h_
#define __LogSupplyStocks_h_

#include "Extractors.h"
#include "FilterHelper.h"
#include <map>

namespace extractors
{

// =============================================================================
/** @class  LogSupplyStocks
    @brief  LogSupplyStocks
*/
// Created: FPO 2011-05-04
// =============================================================================
class LogSupplyStocks : public Extractor< NumericValue >
{
public:
    //! @name Types
    //@{
    enum { has_parameter = true };
    //@}

public:
    //! @name Constructors/Destructor
    //@{
                 LogSupplyStocks();
    explicit LogSupplyStocks( xml::xistream& xis );
    virtual     ~LogSupplyStocks();
    //@}

    //! @name Operations
    //@{
    bool HasValue( const sword::SimToClient& wrapper ) const
    {
        return ( wrapper.message().has_log_supply_state() && wrapper.message().log_supply_state().has_stocks() );
    }
    float Extract( const sword::SimToClient& wrapper );
    //@}

private:
    //! @name Member data
    //@{
       FilterHelper< int > filter_;
       std::map< int, int > logSupplyStocks_;
    //@}
};

}

#endif // __LogSupplyStocks_h_
