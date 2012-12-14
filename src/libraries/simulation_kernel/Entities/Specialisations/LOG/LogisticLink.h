// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef __LogisticLink_h_
#define __LogisticLink_h_

#include "MIL.h"
#include "LogisticLink_ABC.h"

class PHY_DotationCategory;
class MIL_AutomateLOG;

namespace sword {
    class MissionParameter;
}

namespace logistic {
    class LogisticHierarchyOwner_ABC;

// =============================================================================
/** @class  LogisticLink
    @brief  LogisticLink
*/
// Created: NLD 2011-01-05
// =============================================================================
class LogisticLink : public LogisticLink_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             LogisticLink( const LogisticHierarchyOwner_ABC& owner, MIL_AutomateLOG& superior, bool useQuotas, xml::xistream& xis );
             LogisticLink( const LogisticHierarchyOwner_ABC& owner, MIL_AutomateLOG& superior, bool useQuotas );
    virtual ~LogisticLink();
    //@}

    //! @name Operations
    //@{
    virtual MIL_AutomateLOG& GetSuperior() const;

    virtual double ConsumeQuota( const PHY_DotationCategory& dotationCategory, double quantity, const T_Requesters& requesters ); // Return quantity consumed
    virtual void   ReturnQuota ( const PHY_DotationCategory& dotationCategory, double quantity );
    //@}

    //! @name Serialization
    //@{
    virtual void WriteODB( xml::xostream& xos ) const;
    //@}

    //! @name CheckPoint
    //@{
    template< typename Archive > void serialize( Archive&, const unsigned int );
    //@}

    //! @name Network
    //@{
    virtual void SendFullState   ( unsigned int context = 0 ) const;
    virtual void SendChangedState() const;

    virtual void OnReceiveChangeQuotas( const sword::MissionParameter& message );
    //@}

private:
    //! @name Helpers
    //@{
    void WriteQuotas( xml::xostream& xos ) const;
    void ReadQuota( xml::xistream& xis );
    void SendQuotas( unsigned int context = 0 ) const;

    T_Requesters ComputeNotifications( T_Requesters& notifications, T_Requesters requesters ) const;
    //@}

public:
    //! @name Types
    //@{
    struct sDotationQuota
    {
        double quota_;
        double quotaThreshold_;
        T_Requesters notifications_;

        bool operator==( const struct sDotationQuota& rhs ) const
        {
            return quota_ == rhs.quota_ && quotaThreshold_ == rhs.quotaThreshold_;
        }
    };

    typedef std::map< const PHY_DotationCategory*, sDotationQuota > T_DotationQuotas;
    //@}

private:
    const LogisticHierarchyOwner_ABC* owner_;
    const bool useQuotas_;
    MIL_AutomateLOG* superior_;
    T_DotationQuotas quotas_;

    // Network
    mutable bool quotasUpdated_;

private:
    template< typename Archive > friend  void save_construct_data( Archive& archive, const LogisticLink*, const unsigned int /*version*/ );
    template< typename Archive > friend  void load_construct_data( Archive& archive, LogisticLink*, const unsigned int /*version*/ );
};

} // namespace logistic

BOOST_CLASS_EXPORT_KEY( logistic::LogisticLink );

#endif // __LogisticLink_h_
