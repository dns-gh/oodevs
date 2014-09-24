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
#include <tools/Map.h>
#include <boost/serialization/export.hpp>
#include <boost/serialization/optional.hpp>
#include <boost/optional.hpp>

class PHY_DotationCategory;
class MIL_Agent_ABC;
class MIL_CheckPointInArchive;
class MIL_CheckPointOutArchive;
class MIL_ItineraryParameter;

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
    explicit SupplyRequestContainer( const boost::shared_ptr< SupplyRequestBuilder_ABC >& builder );
             SupplyRequestContainer();
    virtual ~SupplyRequestContainer();
    //@}

    void ResetConsignsForConvoyPion( const MIL_Agent_ABC& pion );
    void ResetConsignsForProvider( const MIL_Agent_ABC& pion );
    void DeleteRequestsForRequester( const MIL_AgentPion& pion );
    void ResetConsign();

    //! @name SupplyRequestContainer_ABC
    //@{
    virtual void SetTransportersProvider      ( SupplySupplier_ABC* provider );
    virtual void SetTransporters              ( const T_Transporters& transporters );
    virtual void SetPathToRecipient           ( SupplyRecipient_ABC& recipient, const sword::Pathfind& pathfind );
    virtual void SetPathToTransportersProvider( const sword::Pathfind& pathfind );
    virtual void SetPathToSupplier            ( const sword::Pathfind& pathfind );
    virtual void SetConvoyFactory             ( const SupplyConvoyFactory_ABC& convoyFactory );
    virtual void AddResource                  ( SupplyRecipient_ABC& recipient, const MIL_AgentPion& pion,
                                               const boost::shared_ptr< SupplyResource_ABC >& resource, double quantity );
    //@}

    //! @name SupplyRequestParameters_ABC
    //@{
    virtual       SupplySupplier_ABC*                              GetTransportersProvider      () const;
    virtual const T_Transporters&                                  GetTransporters              () const;
    virtual const SupplyConvoyFactory_ABC&                         GetConvoyFactory             () const;
    virtual       std::vector< boost::shared_ptr< MT_Vector2D > >  GetPathToTransportersProvider( const MT_Vector2D& start, const MT_Vector2D& end ) const;
    virtual       std::vector< boost::shared_ptr< MT_Vector2D > >  GetPathToSupplier            ( const MT_Vector2D& start, const MT_Vector2D& end ) const;
    virtual       std::vector< boost::shared_ptr< MT_Vector2D > >  GetPathToRecipient( const MT_Vector2D& start, SupplyRecipient_ABC& recipient ) const;

    virtual void ToSupplierItinerary( sword::Pathfind& pathfind ) const;
    virtual void ToTransportersItinerary( sword::Pathfind& pathfind ) const;
    virtual void ToRecipientItinerary( SupplyRecipient_ABC& recipient, sword::Pathfind& pathfind ) const;
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
    //! @name Tools
    //@{
    typedef tools::Map< const PHY_DotationCategory*, boost::shared_ptr< SupplyRequest_ABC > > T_Requests;
    typedef tools::Map< SupplyRecipient_ABC*, T_Requests >                                    T_RecipientRequests;
    typedef tools::Map< SupplySupplier_ABC*, boost::shared_ptr< SupplyConsign_ABC > >         T_Consigns;
    typedef boost::shared_ptr< MIL_ItineraryParameter >                                       T_Itinerary;
    typedef tools::Map< SupplyRecipient_ABC*, T_Itinerary >                                   T_RecipientItineraries;
    //@}

private:
    boost::shared_ptr< SupplyRequestBuilder_ABC > builder_;
    boost::shared_ptr< SupplyRequestDispatcher_ABC > dispatcher_;
    T_RecipientRequests requests_;
    T_Consigns consigns_;

    // Parameters
    SupplySupplier_ABC* transportersProvider_; // Default to supplier
    T_Transporters transporters_;
    T_RecipientItineraries recipientPaths_;
    T_Itinerary transportersProviderPath_;
    T_Itinerary supplierPath_;
    const SupplyConvoyFactory_ABC* convoyFactory_;
};

}

BOOST_CLASS_EXPORT_KEY( logistic::SupplyRequestContainer )

#endif // __SupplyRequestContainer_h_
