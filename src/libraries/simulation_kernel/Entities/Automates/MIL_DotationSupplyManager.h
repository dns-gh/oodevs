// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Automates/MIL_Automate.h $
// $Author: Nld $
// $Modtime: 12/05/05 17:40 $
// $Revision: 17 $
// $Workfile: MIL_Automate.h $
//
// *****************************************************************************

#ifndef __MIL_DotationSupplyManager_h_
#define __MIL_DotationSupplyManager_h_

#include "MIL.h"
#include "Entities/Agents/Roles/Logistic/SupplyRecipient_ABC.h"
#include <boost/noncopyable.hpp>
#include <boost/serialization/export.hpp>

class MIL_Automate;
class PHY_DotationCategory;

namespace logistic 
{
    class SupplyRequestBuilder_ABC;
    class SupplyRequestContainer;
}

// =============================================================================
// @class  MIL_Automate
// Created: JVT 2004-08-03
// =============================================================================
class MIL_DotationSupplyManager : public logistic::SupplyRecipient_ABC
                                , private boost::noncopyable
{
public:
             MIL_DotationSupplyManager( MIL_Automate& automate );
             MIL_DotationSupplyManager();
    virtual ~MIL_DotationSupplyManager();

    //! @name CheckPoints
    //@{
    template < typename Archive > void serialize( Archive&, const unsigned int );
    //@}

    //! @name Operations
    //@{
    void Update();
    void Clean ();
    //@}

    //! @name Network
    //@{
    void SendChangedState() const;
    void SendFullState   () const;
    //@}

    //! @name SupplyRecipient_ABC
    //@{
    virtual const MT_Vector2D&   GetPosition() const;
    virtual const MIL_AgentPion& GetPC      () const;
    virtual void OnSupplyCanceled();
    virtual void OnSupplyDone    ();
    virtual void Serialize( sword::AutomatId& msg ) const;
    //@}

    //! @name Notifications
    //@{
    void NotifyDotationSupplyNeeded( const PHY_DotationCategory& dotationCategory );
    void RequestDotationSupply     ();
    //@}

private:
    MIL_Automate* pAutomate_;

    bool                     bDotationSupplyNeeded_;
    bool                     bDotationSupplyExplicitlyRequested_;
    boost::shared_ptr< logistic::SupplyRequestBuilder_ABC > supplyRequestBuilder_;
    std::auto_ptr< logistic::SupplyRequestContainer > supplyRequests_;
    unsigned int                     nTickRcDotationSupplyQuerySent_;
};

BOOST_CLASS_EXPORT_KEY2( MIL_DotationSupplyManager, "MIL_DotationSupplyManager" )

#endif // MIL_DotationSupplyManager
