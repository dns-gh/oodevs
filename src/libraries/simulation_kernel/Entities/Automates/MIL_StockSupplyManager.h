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
#include <boost/serialization/export.hpp>

class PHY_DotationCategory;
class PHY_SupplyStockState;
class MIL_Automate;
template < typename T > class PHY_ActionLogistic;
namespace Common { class MsgMissionParameters; }

// =============================================================================
// @class  MIL_StockSupplyManager
// Created: JVT 2004-08-03
// =============================================================================
class MIL_StockSupplyManager
{
    MT_COPYNOTALLOWED( MIL_StockSupplyManager )

public:
             MIL_StockSupplyManager( MIL_Automate& automate );
             MIL_StockSupplyManager();
    virtual ~MIL_StockSupplyManager();

    //! @name CheckPoints
    //@{
    template < typename Archive > void serialize( Archive&, const uint );
    //@}
    
    //! @name Operations
    //@{
    void Update();
    void Clean ();
    //@}

    //! @name Network
    //@{
    void OnReceiveMsgLogSupplyPushFlow( const Common::MsgMissionParameters& msg );
    void OnReceiveMsgLogSupplyPullFlow( const Common::MsgMissionParameters& msg );

    void SendChangedState() const;
    void SendFullState   () const;
    //@}

    //! @name Logistic : supply
    void NotifyStockSupplyNeeded  ( const PHY_DotationCategory& dotationCategory );
    void NotifyStockSupplied      ( const PHY_SupplyStockState& supplyState );
    void NotifyStockSupplyCanceled( const PHY_SupplyStockState& supplyState );
    //@}

private:
    //! @name Types
    //@{
    typedef std::set< PHY_SupplyStockState* >     T_SupplyStockStateSet;
    typedef T_SupplyStockStateSet::iterator       IT_SupplyStockStateSet;
    typedef T_SupplyStockStateSet::const_iterator CIT_SupplyStockStateSet;
    //@}

private:
    //! @name Tools
    //@{
    bool IsSupplyInProgress    ( const PHY_DotationCategory& dotationCategory ) const;
    void RemoveSupplyStockState( const PHY_SupplyStockState& supplyState );
    //@}

private:
    MIL_Automate* pAutomate_;

    bool                  bStockSupplyNeeded_;
    PHY_SupplyStockState* pExplicitStockSupplyState_;
    T_SupplyStockStateSet manualSupplyStates_;
    uint                  nTickRcStockSupplyQuerySent_;
};

BOOST_CLASS_EXPORT_KEY2( MIL_StockSupplyManager, "MIL_StockSupplyManager" )

#endif // __MIL_StockSupplyManager_h_

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
#include <boost/serialization/export.hpp>

class PHY_DotationCategory;
class PHY_SupplyStockState;
class MIL_Automate;
template < typename T > class PHY_ActionLogistic;
namespace Common { class MsgMissionParameters; }

// =============================================================================
// @class  MIL_StockSupplyManager
// Created: JVT 2004-08-03
// =============================================================================
class MIL_StockSupplyManager
{
    MT_COPYNOTALLOWED( MIL_StockSupplyManager )

public:
             MIL_StockSupplyManager( MIL_Automate& automate );
             MIL_StockSupplyManager();
    virtual ~MIL_StockSupplyManager();

    //! @name CheckPoints
    //@{
    template < typename Archive > void serialize( Archive&, const uint );
    //@}
    
    //! @name Operations
    //@{
    void Update();
    void Clean ();
    //@}

    //! @name Network
    //@{
    void OnReceiveMsgLogSupplyPushFlow( const Common::MsgMissionParameters& msg );
    void OnReceiveMsgLogSupplyPullFlow( const Common::MsgMissionParameters& msg );

    void SendChangedState() const;
    void SendFullState   () const;
    //@}

    //! @name Logistic : supply
    void NotifyStockSupplyNeeded  ( const PHY_DotationCategory& dotationCategory );
    void NotifyStockSupplied      ( const PHY_SupplyStockState& supplyState );
    void NotifyStockSupplyCanceled( const PHY_SupplyStockState& supplyState );
    //@}

private:
    //! @name Types
    //@{
    typedef std::set< PHY_SupplyStockState* >     T_SupplyStockStateSet;
    typedef T_SupplyStockStateSet::iterator       IT_SupplyStockStateSet;
    typedef T_SupplyStockStateSet::const_iterator CIT_SupplyStockStateSet;
    //@}

private:
    //! @name Tools
    //@{
    bool IsSupplyInProgress    ( const PHY_DotationCategory& dotationCategory ) const;
    void RemoveSupplyStockState( const PHY_SupplyStockState& supplyState );
    //@}

private:
    MIL_Automate* pAutomate_;

    bool                  bStockSupplyNeeded_;
    PHY_SupplyStockState* pExplicitStockSupplyState_;
    T_SupplyStockStateSet manualSupplyStates_;
    uint                  nTickRcStockSupplyQuerySent_;
};

BOOST_CLASS_EXPORT_KEY2( MIL_StockSupplyManager, "MIL_StockSupplyManager" )

#endif // __MIL_StockSupplyManager_h_

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
#include <boost/serialization/export.hpp>

class PHY_DotationCategory;
class PHY_SupplyStockState;
class MIL_Automate;
template < typename T > class PHY_ActionLogistic;
namespace Common { class MsgMissionParameters; }

// =============================================================================
// @class  MIL_StockSupplyManager
// Created: JVT 2004-08-03
// =============================================================================
class MIL_StockSupplyManager
{
    MT_COPYNOTALLOWED( MIL_StockSupplyManager )

public:
             MIL_StockSupplyManager( MIL_Automate& automate );
             MIL_StockSupplyManager();
    virtual ~MIL_StockSupplyManager();

    //! @name CheckPoints
    //@{
    template < typename Archive > void serialize( Archive&, const uint );
    //@}
    
    //! @name Operations
    //@{
    void Update();
    void Clean ();
    //@}

    //! @name Network
    //@{
    void OnReceiveMsgLogSupplyPushFlow( const Common::MsgMissionParameters& msg );
    void OnReceiveMsgLogSupplyPullFlow( const Common::MsgMissionParameters& msg );

    void SendChangedState() const;
    void SendFullState   () const;
    //@}

    //! @name Logistic : supply
    void NotifyStockSupplyNeeded  ( const PHY_DotationCategory& dotationCategory );
    void NotifyStockSupplied      ( const PHY_SupplyStockState& supplyState );
    void NotifyStockSupplyCanceled( const PHY_SupplyStockState& supplyState );
    //@}

private:
    //! @name Types
    //@{
    typedef std::set< PHY_SupplyStockState* >     T_SupplyStockStateSet;
    typedef T_SupplyStockStateSet::iterator       IT_SupplyStockStateSet;
    typedef T_SupplyStockStateSet::const_iterator CIT_SupplyStockStateSet;
    //@}

private:
    //! @name Tools
    //@{
    bool IsSupplyInProgress    ( const PHY_DotationCategory& dotationCategory ) const;
    void RemoveSupplyStockState( const PHY_SupplyStockState& supplyState );
    //@}

private:
    MIL_Automate* pAutomate_;

    bool                  bStockSupplyNeeded_;
    PHY_SupplyStockState* pExplicitStockSupplyState_;
    T_SupplyStockStateSet manualSupplyStates_;
    uint                  nTickRcStockSupplyQuerySent_;
};

BOOST_CLASS_EXPORT_KEY2( MIL_StockSupplyManager, "MIL_StockSupplyManager" )

#endif // __MIL_StockSupplyManager_h_

