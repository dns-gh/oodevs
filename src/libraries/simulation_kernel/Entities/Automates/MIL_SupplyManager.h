// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#ifndef __MIL_SupplyManager_h_
#define __MIL_SupplyManager_h_

#include "Entities/Agents/Roles/Logistic/SupplyRecipient_ABC.h"
#include "Entities/Orders/MIL_Report.h"
#include <map>

class PHY_DotationCategory;

// =============================================================================
/** @class  MIL_SupplyManager
    @brief  MIL_SupplyManager
*/
// Created: MCO 2012-12-19
// =============================================================================
class MIL_SupplyManager : public logistic::SupplyRecipient_ABC
{
protected:
    //! @name Constructors/Destructor
    //@{
             MIL_SupplyManager();
    virtual ~MIL_SupplyManager();
    //@}

    //! @name Operations
    //@{
    void Clean();
    bool SendSupplyNeededReport();
    friend class boost::serialization::access;
    template < typename Archive > void serialize( Archive& archive, const unsigned int )
    {
        archive & boost::serialization::base_object< logistic::SupplyRecipient_ABC >( *this );
    }
    //@}

private:
    //! @name Operations
    //@{
    virtual void NotifySuperiorNotAvailable( const PHY_DotationCategory& dotationCategory, const T_Requesters& requesters );
    virtual void NotifyStockNotAvailable( const PHY_DotationCategory& dotationCategory, const T_Requesters& requesters );
    //@}

    //! @name Helpers
    //@{
    void PostEvent( MIL_Report::E_DecisionalReport report, const PHY_DotationCategory& dotationCategory, const T_Requesters& requesters );
    //@}

private:
    //! @name Types
    //@{
    typedef std::map< std::pair< MIL_Report::E_DecisionalReport, const PHY_DotationCategory* >, T_Requesters > T_Notifications;
    //@}

private:
    //! @name Member data
    //@{
    T_Notifications currentNotifications_;
    T_Notifications previousNotifications_;
    unsigned int nTickRcSupplyQuerySent_;
    //@}
};

BOOST_CLASS_EXPORT_KEY( MIL_SupplyManager );

#endif // __MIL_SupplyManager_h_
