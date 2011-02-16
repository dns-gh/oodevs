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
#include <boost/noncopyable.hpp>
#include <boost/serialization/export.hpp>

class MIL_Automate;
class MIL_AutomateLOG;
class PHY_SupplyDotationState;
class PHY_DotationCategory;

// =============================================================================
// @class  MIL_Automate
// Created: JVT 2004-08-03
// =============================================================================
class MIL_DotationSupplyManager : private boost::noncopyable
{

public:
    //! @name Types
    //@{
    typedef std::map< const MIL_AutomateLOG*, PHY_SupplyDotationState* > T_SupplyDotationStateMap;
    typedef T_SupplyDotationStateMap::iterator                             IT_SupplyDotationStateMap;
    typedef T_SupplyDotationStateMap::const_iterator                       CIT_SupplyDotationStateMap;
    //@}

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

    //! @name Logistic : supply
    //@{
    void NotifyDotationSupplyNeeded( const PHY_DotationCategory& dotationCategory );
    void NotifyDotationSupplied    ( const PHY_SupplyDotationState& supplyState );
    void RequestDotationSupply     ();
    //@}

private:
    MIL_Automate* pAutomate_;

    bool                     bDotationSupplyNeeded_;
    bool                     bDotationSupplyExplicitlyRequested_;
    T_SupplyDotationStateMap dotationSupplyStates_;

    unsigned int                     nTickRcDotationSupplyQuerySent_;
};

BOOST_CLASS_EXPORT_KEY2( MIL_DotationSupplyManager, "MIL_DotationSupplyManager" )

#endif // MIL_DotationSupplyManager
