// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef __SupplyRequestContainer_h_
#define __SupplyRequestContainer_h_

#include "SupplyRequestContainer_ABC.h"
#include "SupplyRequestParameters_ABC.h"
#include "tools/Map.h"
#include <boost/serialization/export.hpp>

class PHY_DotationCategory;
class MIL_Agent_ABC;
class MIL_CheckPointInArchive;
class MIL_CheckPointOutArchive;

namespace logistic
{
    class SupplyRequest_ABC;
    class SupplyRequestBuilder_ABC;
    class SupplyRequestDispatcher_ABC;
    class SupplyConsign_ABC;

// =============================================================================
/** @class  SupplyRequestContainer
    @brief  SupplyRequestContainer
*/
// Created: NLD 2011-01-10
// =============================================================================
class SupplyRequestContainer : public SupplyRequestContainer_ABC
                             , public SupplyRequestParameters_ABC
{
public:
    //! @name Types
    //@{
    typedef std::vector< std::pair< const PHY_ComposanteTypePion* , unsigned int > > T_Transporters;
    //@}

public:
    //! @name Constructors/Destructor
    //@{
    explicit SupplyRequestContainer( boost::shared_ptr< SupplyRequestBuilder_ABC > builder );
             SupplyRequestContainer();
    virtual ~SupplyRequestContainer();
    //@}

    void ResetConsignsForConvoyPion( const MIL_AgentPion& pion );
    void ResetConsignsForProvider( const MIL_Agent_ABC& pion );
    void ResetConsign();

    //! @name SupplyRequestContainer_ABC
    //@{
    virtual void SetTransportersProvider      ( SupplySupplier_ABC* provider );
    virtual void SetTransporters              ( const T_Transporters& transporters );
    virtual void SetPathToRecipient           ( SupplyRecipient_ABC& recipient, const T_PointVector& wayPoints );
    virtual void SetPathToTransportersProvider( const T_PointVector& wayPoints );
    virtual void SetPathToSupplier            ( const T_PointVector& wayPoints );
    virtual void SetConvoyFactory             ( const SupplyConvoyFactory_ABC& convoyFactory );
    virtual void AddResource                  ( SupplyRecipient_ABC& recipient, const MIL_AgentPion& pion, boost::shared_ptr< SupplyResource_ABC > resource, double quantity );
    //@}

    //! @name SupplyRequestParameters_ABC
    //@{
    virtual       SupplySupplier_ABC*      GetTransportersProvider      () const;
    virtual const T_Transporters&          GetTransporters              () const;
    virtual const T_PointVector&           GetPathToRecipient           ( SupplyRecipient_ABC& recipient ) const;
    virtual const T_PointVector&           GetPathToTransportersProvider() const;
    virtual const T_PointVector&           GetPathToSupplier            () const;
    virtual const SupplyConvoyFactory_ABC& GetConvoyFactory             () const;
    //@}

    //! @name Main
    bool Execute    ( SupplyRequestDispatcher_ABC& dispatcher );
    bool Update     (); // Return true if all consigns are finished
    void Clean      ();
    //@}

    //! @name Network - A refactorer
    //@{
    void SendChangedState() const;
    void SendFullState   () const;
    //@}
    
    //! @name CheckPoints
    //@{
    void serialize( MIL_CheckPointInArchive&, const unsigned int );
    void serialize( MIL_CheckPointOutArchive&, const unsigned int );
    //@}

private:
    //! @name Tools
    //@{
    void Prepare();
    //@}

private:
    typedef tools::Map< const PHY_DotationCategory*, boost::shared_ptr< SupplyRequest_ABC > > T_Requests;
    typedef tools::Map< SupplyRecipient_ABC*, T_Requests >                                    T_RecipientRequests;
    typedef tools::Map< SupplySupplier_ABC*, boost::shared_ptr< SupplyConsign_ABC > >         T_Consigns;
    typedef tools::Map< SupplyRecipient_ABC*, T_PointVector >                                 T_RecipientPaths;

private:
    boost::shared_ptr< SupplyRequestBuilder_ABC > builder_;
    boost::shared_ptr< SupplyRequestDispatcher_ABC > dispatcher_;
    T_RecipientRequests requests_;
    T_Consigns consigns_;

    // Parameters
    SupplySupplier_ABC* transportersProvider_; // Default to supplier
    T_Transporters transporters_;
    T_RecipientPaths recipientPaths_;
    T_PointVector transportersProviderPath_;
    T_PointVector supplierPath_;
    const SupplyConvoyFactory_ABC* convoyFactory_;
};

}

BOOST_CLASS_EXPORT_KEY( logistic::SupplyRequestContainer )

#endif // __SupplyRequestContainer_h_
