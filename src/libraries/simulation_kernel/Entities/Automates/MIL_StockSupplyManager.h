// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Specialisations/Log/MIL_StockSupplyManager.h $
// $Author: Jvt $
// $Modtime: 14/04/05 11:33 $
// $Revision: 12 $
// $Workfile: MIL_StockSupplyManager.h $
//
// *****************************************************************************

#ifndef __MIL_StockSupplyManager_h_
#define __MIL_StockSupplyManager_h_

#include "MIL.h"
#include "Entities/Agents/Roles/Logistic/SupplyRecipient_ABC.h"
#include <boost/noncopyable.hpp>
#include <boost/serialization/export.hpp>

class PHY_DotationCategory;
class MIL_Automate;
template < typename T > class PHY_ActionLogistic;

namespace logistic 
{
    class SupplyRequestBuilder_ABC;
    class SupplyRequestContainer;
    class SupplySupplier_ABC;
}

namespace sword
{
    class PullFlowParameters;
}

// =============================================================================
// @class  MIL_StockSupplyManager
// Created: JVT 2004-08-03
// =============================================================================
class MIL_StockSupplyManager : public logistic::SupplyRecipient_ABC
                             , private boost::noncopyable
{
public:
    //! @name Constructors/Destructor
    //@{
             MIL_StockSupplyManager( MIL_Automate& automate );
             MIL_StockSupplyManager();
    virtual ~MIL_StockSupplyManager();
    //@}

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
    void OnReceiveLogSupplyPullFlow( const sword::PullFlowParameters& parameters, logistic::SupplySupplier_ABC& supplier );

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

    //! @name Logistic
    void NotifyStockSupplyNeeded( const PHY_DotationCategory& dotationCategory );
    //@}

private:
    //! @name Helpers
    //@{
    bool IsSupplyInProgress( const PHY_DotationCategory& dotationCategory ) const;
    //@}

private:
    //! @name Types
    //@{
    typedef std::list< boost::shared_ptr< logistic::SupplyRequestContainer > > T_SupplyRequests;
    //@}

private:
    //! @name Member data
    //@{
    MIL_Automate*         pAutomate_;
    bool                  bStockSupplyNeeded_;
    boost::shared_ptr< logistic::SupplyRequestBuilder_ABC > supplyRequestBuilder_;
    std::auto_ptr< logistic::SupplyRequestContainer > autoSupplyRequest_;
    T_SupplyRequests manualSupplyRequests_;
    unsigned int          nTickRcStockSupplyQuerySent_;
    //@}
};

BOOST_CLASS_EXPORT_KEY2( MIL_StockSupplyManager, "MIL_StockSupplyManager" )

#endif // __MIL_StockSupplyManager_h_
