// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef __LogisticHierarchyOwner_ABC_h_
#define __LogisticHierarchyOwner_ABC_h_

#include <vector>

class MIL_AgentPion;

namespace sword
{
    class ParentEntity;
}

class PHY_DotationCategory;

namespace logistic
{
    class LogisticLink_ABC;

// =============================================================================
/** @class  LogisticHierarchyOwner_ABC
    @brief  LogisticHierarchyOwner_ABC
*/
// Created: NLD 2011-01-10
// =============================================================================
class LogisticHierarchyOwner_ABC
{
public:
    //! @name Types
    //@{
    typedef std::vector< const MIL_AgentPion* > T_Requesters;
    //@}

public:
    //! @name Constructors/Destructor
    //@{
             LogisticHierarchyOwner_ABC() {}
    virtual ~LogisticHierarchyOwner_ABC() {}
    //@}

    //! @name Events
    //@{
    virtual void NotifyLinkAdded( const LogisticLink_ABC& link ) const = 0;
    virtual void NotifyLinkRemoved( const LogisticLink_ABC& link ) const = 0;

    virtual void NotifyQuotaThresholdReached( const PHY_DotationCategory& dotationCategory ) const = 0;
    virtual void NotifyQuotaExceeded( const PHY_DotationCategory& dotationCategory, const T_Requesters& requesters ) const = 0;
    //@}

    //! @name Accessors
    //@{
    virtual unsigned int GetLogisticId() const = 0;
    //@}

    //! @name Serialization
    //@{
    virtual void Serialize( sword::ParentEntity& msg ) const = 0;
    //@}

    //! @name CheckPoint
    //@{
    template< typename Archive > void serialize( Archive&, const unsigned int ) {}
    //@}
};

} // end namespace LogisticHierarchyOwner

#endif // __LogisticHierarchyOwner_ABC_h_
