// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef __LogisticHierarchy_ABC_h_
#define __LogisticHierarchy_ABC_h_

#include "NetworkMessageSender_ABC.h"

class MIL_AutomateLOG;

namespace tools 
{
    template< typename T > class Iterator;
}

namespace logistic {

class LogisticLink_ABC;

// =============================================================================
/** @class  LogisticHierarchy_ABC
    @brief  LogisticHierarchy_ABC
*/
// Created: NLD 2011-01-10
// =============================================================================
class LogisticHierarchy_ABC : private boost::noncopyable
{
public:
    //! @name Constructors/Destructor
    //@{
             LogisticHierarchy_ABC() {}
    virtual ~LogisticHierarchy_ABC() {}
    //@}

    //! @name Operations
    //@{
    virtual bool             HasSuperior() const = 0;
    virtual MIL_AutomateLOG* GetPrimarySuperior() const = 0;

    // Returns the superior by priority : first has highest priority
    virtual tools::Iterator< const LogisticLink_ABC& > CreateSuperiorLinksIterator() const = 0;
    virtual tools::Iterator< MIL_AutomateLOG& >        CreateSuperiorsIterator    () const = 0;

    virtual const boost::shared_ptr< LogisticLink_ABC > FindSuperiorLink( const MIL_AutomateLOG& superior ) const = 0;

    virtual void ChangeLinks( const std::vector< MIL_AutomateLOG* >& superiors ) = 0;
    //@}

    //! @name Serialization
    //@{
    virtual void WriteODB( xml::xostream& xos ) const = 0;
    //@}

    //! @name CheckPoint
    //@{
    template< typename Archive > void serialize( Archive&, const unsigned int ) {}
    //@}
};

} // end namespace LogisticHierarchy

#endif // __LogisticHierarchy_ABC_h_
