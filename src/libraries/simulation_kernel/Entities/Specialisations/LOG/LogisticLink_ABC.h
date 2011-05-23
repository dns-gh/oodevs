// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef __LogisticLink_ABC_h_
#define __LogisticLink_ABC_h_

#include "NetworkMessageSender_ABC.h"

//$$$ TMP, remplacer par une interface
class MIL_AutomateLOG;
class PHY_DotationCategory;

namespace sword 
{
    class MissionParameter;
}

namespace logistic {

// =============================================================================
/** @class  LogisticHierarchy_ABC
    @brief  LogisticHierarchy_ABC
*/
// Created: NLD 2011-01-10
// =============================================================================
class LogisticLink_ABC : public network::NetworkMessageSender_ABC
                       , private boost::noncopyable
{

public:
    //! @name Constructors/Destructor
    //@{
             LogisticLink_ABC() {}
    virtual ~LogisticLink_ABC() {}
    //@}

    //! @name Operations
    //@{
    virtual MIL_AutomateLOG& GetSuperior () const = 0;
    virtual void             ConsumeQuota( const PHY_DotationCategory& dotationCategory, double quotaConsumed ) = 0;
    virtual double           GetQuota    ( const PHY_DotationCategory& dotationCategory ) const = 0;
    //@}

    //! @name Serialization
    //@{
    virtual void WriteODB( xml::xostream& xos ) const = 0;
    //@}

    //! @name Network
    //@{
    virtual void OnReceiveChangeQuotas( const sword::MissionParameter& message ) = 0;
    //@}

    //! @name CheckPoint
    //@{
    template< typename Archive > void serialize( Archive&, const unsigned int ) {}
    //@}
};

} // end namespace LogisticHierarchy

#endif // __LogisticLink_ABC_h_
