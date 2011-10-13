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

namespace logistic {
    class SupplyConsign_ABC;

// =============================================================================
/** @class  SupplyConsign_ABC
    @brief  SupplyConsign_ABC
*/
// Created: NLD 2011-01-10
// =============================================================================
class SupplyConvoysObserver_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             SupplyConvoysObserver_ABC() {}
    virtual ~SupplyConvoysObserver_ABC() {}
    //@}

    //! @name Operations
    //@{
    virtual void OnSupplyConvoyArriving( boost::shared_ptr< const SupplyConsign_ABC > supplyConsign ) = 0;
    virtual void OnSupplyConvoyLeaving ( boost::shared_ptr< const SupplyConsign_ABC > supplyConsign ) = 0;
    //@}
};

} // end namespace logistic

#endif // __SupplyConvoysObserver_ABC_h_
