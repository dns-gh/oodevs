// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Agents/Roles/Logistic/PHY_SupplyConsign_ABC.h $
// $Author: Jvt $
// $Modtime: 11/04/05 17:20 $
// $Revision: 5 $
// $Workfile: PHY_SupplyConsign_ABC.h $
//
// *****************************************************************************

#ifndef __PHY_SupplyConsign_ABC_h_
#define __PHY_SupplyConsign_ABC_h_

#include "Entities/Automates/MIL_Automate.h"
#include "MT_Tools/MT_Vector2DTypes.h"

namespace client
{
    class LogSupplyHandlingUpdate;
}

class MIL_AutomateLOG;
class MIL_Automate;
class PHY_DotationCategory;

// =============================================================================
// @class  PHY_SupplyConsign_ABC
// Created: JVT 2004-08-03
// =============================================================================
class PHY_SupplyConsign_ABC : private boost::noncopyable
{
public:
    //! @name Constructors/Destructor
    //@{
             PHY_SupplyConsign_ABC( MIL_AutomateLOG& supplyingAutomate, MIL_AutomateLOG& convoyingAutomate, MIL_Automate& stockSupplier );
             PHY_SupplyConsign_ABC();
    virtual ~PHY_SupplyConsign_ABC();
    //@}

    //! @name CheckPoints
    //@{
    template< typename Archive > void serialize( Archive&, const unsigned int );
    //@}

    //! @name Operations
    //@{
    virtual bool Update() = 0;
    void Clean();
    bool HasChanged() const;
    //@}

    //! @name Types
    //@{
    typedef std::map< const PHY_DotationCategory*, double > T_MerchandiseToConvoyMap;
    //@}

    //! @name Accessors
    //@{
    MIL_AutomateLOG& GetSupplier() const;
    MIL_AutomateLOG& GetConvoyer() const;
    MIL_Automate& GetStockSupplier() const;
    virtual const MIL_Automate* GetNextSupplied() const = 0;
    virtual const T_PointVector* GetWayPointsToGoBack() const = 0;
    virtual const T_PointVector* GetWayPointsToGoToNextSupplied() const = 0;

    virtual void GetMerchandiseToConvoy( T_MerchandiseToConvoyMap& container ) const = 0;
    virtual void RemoveConvoyedMerchandise( const PHY_DotationCategory& dotationCategory, double rNbrDotations ) = 0;
    virtual void AddConvoyedMerchandise( const PHY_DotationCategory& dotationCategory, double rNbrDotations ) = 0;
    virtual void CancelMerchandiseOverheadReservation() = 0;
    //@}

    //! @name Network
    //@{
    void SendChangedState( client::LogSupplyHandlingUpdate& asn ) const;
    virtual void SendFullState( client::LogSupplyHandlingUpdate& asn ) const;
    static void SendDefaultState( client::LogSupplyHandlingUpdate& asn );
    //@}

protected:
    //! @name Types
    //@{
    enum E_State
    {
        eConvoyWaitingForTransporters,
        eConvoyForming,
        eConvoyGoingToLoadingPoint,
        eConvoyLoading,
        eConvoyGoingToUnloadingPoint,
        eConvoyUnloading,
        eConvoyGoingBackToFormingPoint,
        eFinished
    };
    //@}

protected:
    //! @name
    //@{
    E_State GetState() const;
    void SetState( E_State nNewState );
    //@}

private:
    //! @name Member data
    //@{
    MIL_AutomateLOG* pSupplyingAutomate_;
    MIL_AutomateLOG* pConvoyingAutomate_;
    const MIL_Automate* pSuppliedAutomate_;
    E_State nState_;
    bool bHasChanged_;
    MIL_Automate*      pStockSupplier_;
    //@}

protected:
    //! @name Member data
    //@{
    int nTimer_;
    //@}
};

// -----------------------------------------------------------------------------
// Name: PHY_SupplyConsign_ABC::serialize
// Created: JVT 2005-04-11
// -----------------------------------------------------------------------------
template< typename Archive >
void PHY_SupplyConsign_ABC::serialize( Archive& file, const unsigned int )
{
    file & pSupplyingAutomate_
         & const_cast< MIL_Automate*& >( pSuppliedAutomate_ )
         & pConvoyingAutomate_
         & nTimer_
         & nState_;
}

#endif // __PHY_SupplyConsign_ABC_h_
