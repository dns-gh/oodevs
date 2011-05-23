// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef __LogisticHierarchy_h_
#define __LogisticHierarchy_h_

#include "MIL.h"
#include "LogisticHierarchy_ABC.h"
#include <deque>

class MIL_AutomateLOG;

namespace logistic {
    class LogisticLink;
    class LogisticHierarchyOwner_ABC;
   
// =============================================================================
/** @class  LogisticHierarchy
    @brief  LogisticHierarchy
*/
// Created: NLD 2011-01-05
// =============================================================================
class LogisticHierarchy : public network::NetworkMessageSender_ABC
                        , public LogisticHierarchy_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
    explicit LogisticHierarchy( const LogisticHierarchyOwner_ABC& owner );
             LogisticHierarchy( const LogisticHierarchyOwner_ABC& owner, MIL_AutomateLOG& nominalSuperior );
             LogisticHierarchy( const LogisticHierarchyOwner_ABC& owner, MIL_AutomateLOG& nominalSuperior, xml::xistream& xis );
    virtual ~LogisticHierarchy();
    //@}

    //! @name Operations
    //@{
    void DisconnectFromHierarchy     ();
    void SwitchToHierarchy           ( const LogisticHierarchy_ABC& hierarchy );
    void SwitchBackToNominalHierarchy();

    virtual bool             HasSuperior() const;
    virtual MIL_AutomateLOG* GetPrimarySuperior() const;

    virtual tools::Iterator< const LogisticLink_ABC& > CreateSuperiorLinksIterator() const;
    virtual tools::Iterator< MIL_AutomateLOG&  >       CreateSuperiorsIterator    () const;

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

    //! @name Network
    //@{
    virtual void SendFullState   () const;
    virtual void SendChangedState() const;
    //@}

private:
    //! @name Helpers
    //@{
    void SendLinks() const;
    //@}

public:
    //! @name Types
    //@{
    typedef std::deque< boost::shared_ptr< LogisticLink_ABC > > T_SuperiorLinks;
    //@}

private:
    //! @name Member data
    //@{
    const LogisticHierarchyOwner_ABC* owner_;

    T_SuperiorLinks superiorLinks_;
    T_SuperiorLinks backupSuperiorLinks_; //$$$ TMP, remplacer par un proxy à terme
    
    // Network
    mutable bool linksUpdated_;
    //@}

    template< typename Archive > friend  void save_construct_data( Archive& archive, const LogisticHierarchy*, const unsigned int /*version*/ );
    template< typename Archive > friend  void load_construct_data( Archive& archive, LogisticHierarchy*, const unsigned int /*version*/ );
};

} // namespace logistic

BOOST_CLASS_EXPORT_KEY( logistic::LogisticHierarchy );

#endif // __LogisticHierarchy_h_
