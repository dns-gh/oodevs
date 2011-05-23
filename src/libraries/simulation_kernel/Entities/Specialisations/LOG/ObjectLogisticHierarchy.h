// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef __ObjectLogisticHierarchy_h_
#define __ObjectLogisticHierarchy_h_

#include "MIL.h"
#include "LogisticHierarchy_ABC.h"

class MIL_AutomateLOG;

namespace logistic {
    class LogisticLink;
   
// =============================================================================
/** @class  ObjectLogisticHierarchy
    @brief  ObjectLogisticHierarchy
*/
// Created: NLD 2011-01-05
// =============================================================================
class ObjectLogisticHierarchy : public LogisticHierarchy_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             ObjectLogisticHierarchy();
             ObjectLogisticHierarchy( MIL_AutomateLOG& superior );
    virtual ~ObjectLogisticHierarchy();
    //@}

    //! @name Operations
    //@{
    virtual bool             HasSuperior() const;
    virtual MIL_AutomateLOG* GetPrimarySuperior() const;

    virtual tools::Iterator< const LogisticLink_ABC& > CreateSuperiorLinksIterator() const;
    virtual tools::Iterator< MIL_AutomateLOG& >        CreateSuperiorsIterator() const;

    virtual const boost::shared_ptr< LogisticLink_ABC > FindSuperiorLink( const MIL_AutomateLOG& superior ) const;

    virtual void ChangeLinks( const std::vector< MIL_AutomateLOG* >& superiors );
    //@}

    //! @name Serialization
    //@{
    virtual void WriteODB( xml::xostream& xos ) const;
    //@}

    //! @name CheckPoint
    //@{
    template< typename Archive > void serialize( Archive&, const unsigned int );
    //@}

private:
    //! @name Member data
    //@{
    boost::shared_ptr< LogisticLink_ABC > superiorLink_;
    //@}
};

} // namespace logistic

BOOST_CLASS_EXPORT_KEY( logistic::ObjectLogisticHierarchy );

#endif // __LogisticHierarchy_h_
