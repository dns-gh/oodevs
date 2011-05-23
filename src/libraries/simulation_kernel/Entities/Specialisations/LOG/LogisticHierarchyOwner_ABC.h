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

namespace sword
{
    class ParentEntity;
}

class PHY_DotationCategory;

namespace logistic {

// =============================================================================
/** @class  LogisticHierarchyOwner_ABC
    @brief  LogisticHierarchyOwner_ABC
*/
// Created: NLD 2011-01-10
// =============================================================================
class LogisticHierarchyOwner_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             LogisticHierarchyOwner_ABC() {}
    virtual ~LogisticHierarchyOwner_ABC() {}
    //@}

    //! @name Events
    //@{
    virtual void NotifyQuotaThresholdReached( const PHY_DotationCategory& dotationCategory ) const = 0;
    //@}

    //! @name Accessors
    //@{
    virtual unsigned int GetID() const = 0;
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
