// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef __SupplyRequestDispatcher_ABC_h_
#define __SupplyRequestDispatcher_ABC_h_

class MIL_AutomateLOG;

namespace logistic {
   class SupplyRequest_ABC;
   class SupplyConsign_ABC;

// =============================================================================
/** @class  SupplyRequestDispatcher_ABC
    @brief  SupplyRequestDispatcher_ABC
*/
// Created: NLD 2011-01-10
// =============================================================================
class SupplyRequestDispatcher_ABC : private boost::noncopyable
{
public:
    //! @name Constructors/Destructor
    //@{
             SupplyRequestDispatcher_ABC() {}
    virtual ~SupplyRequestDispatcher_ABC() {}
    //@}

    //! @name Operations
    //@{
    virtual void Dispatch( SupplyRequest_ABC& request ) = 0;
    virtual bool AllowSupply() = 0;
    //@}

    //! @name CheckPoint
    //@{
    template< typename Archive > void serialize( Archive&, const unsigned int ) {}
    //@}
};

} // end namespace logistic

#endif // __SupplyRequestDispatcher_ABC_h_
