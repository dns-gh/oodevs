// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef __SupplyRequestBuilder_ABC_h_
#define __SupplyRequestBuilder_ABC_h_

namespace logistic {
        class SupplyRequestContainer_ABC;

// =============================================================================
/** @class  SupplyRequestBuilder_ABC
    @brief  SupplyRequestBuilder_ABC
*/
// Created: NLD 2011-01-10
// =============================================================================
class SupplyRequestBuilder_ABC : private boost::noncopyable
{
public:
    //! @name Constructors/Destructor
    //@{
             SupplyRequestBuilder_ABC() {};
    virtual ~SupplyRequestBuilder_ABC() {};
    //@}

    //! @name Operations
    //@{
    virtual void Process( SupplyRequestContainer_ABC& container ) = 0;
    //@}
};

} // end namespace logistic

#endif // __SupplyRequestBuilder_ABC_h_
