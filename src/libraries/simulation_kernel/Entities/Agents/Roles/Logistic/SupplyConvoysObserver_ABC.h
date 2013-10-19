// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef __SupplyConvoysObserver_ABC_h_
#define __SupplyConvoysObserver_ABC_h_

#include <boost/noncopyable.hpp>

namespace logistic
{
    class SupplyConsign_ABC;

// =============================================================================
/** @class  SupplyConsign_ABC
    @brief  SupplyConsign_ABC
*/
// Created: NLD 2011-01-10
// =============================================================================
class SupplyConvoysObserver_ABC : boost::noncopyable
{
public:
    //! @name Constructors/Destructor
    //@{
             SupplyConvoysObserver_ABC() {}
    virtual ~SupplyConvoysObserver_ABC() {}
    //@}

    //! @name Operations
    //@{
    virtual void OnSupplyConvoyArriving( const boost::shared_ptr< const SupplyConsign_ABC >& consign ) = 0;
    virtual void OnSupplyConvoyLeaving ( const boost::shared_ptr< const SupplyConsign_ABC >& consign ) = 0;

    virtual void FinishSuccessfullyWithoutDelay() = 0;
    //@}
};

}

#endif // __SupplyConvoysObserver_ABC_h_
