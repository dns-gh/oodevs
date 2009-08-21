// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Agents/Roles/Dotations/PHY_RolePion_Dotations.h $
// $Author: Nld $
// $Modtime: 11/05/05 18:18 $
// $Revision: 11 $
// $Workfile: PHY_RolePion_Dotations.h $
//
// *****************************************************************************

#ifndef __PHY_RolePion_Dotations_h_
#define __PHY_RolePion_Dotations_h_

#include "MIL.h"
#include "PHY_RoleInterface_Dotations.h"

namespace xml
{
    class xostream;
    class xistream;
}

class MIL_AgentPion;
class PHY_DotationCapacities;
class PHY_DotationCategory;
class PHY_DotationType;
class PHY_Dotation;
class PHY_ConsumptionType;
class PHY_SupplyState;
class PHY_SupplyDotationRequestContainer;
class PHY_DotationGroupContainer;
class PHY_AmmoDotationClass;
class NET_ASN_MsgUnitAttributes;

// =============================================================================
// @class  PHY_RolePion_Dotations
// Created: JVT 2004-08-03
// =============================================================================
class PHY_RolePion_Dotations : public PHY_RoleInterface_Dotations
{

public:
    explicit PHY_RolePion_Dotations( MIL_AgentPion& pion );
    virtual ~PHY_RolePion_Dotations();

    //! @name CheckPoints
    //@{
    BOOST_SERIALIZATION_SPLIT_MEMBER()
    
    void load( MIL_CheckPointInArchive&, const uint );
    void save( MIL_CheckPointOutArchive&, const uint ) const;

    void WriteODB( xml::xostream& xos ) const;
    //@}
    
    //! @name External operations
    //@{
    void ReadOverloading              ( xml::xistream& xis );
    void RegisterDotationsCapacities  ( const PHY_DotationCapacities& capacities );
    void UnregisterDotationsCapacities( const PHY_DotationCapacities& capacities );

    void NotifyCaptured();
    void NotifyReleased();
    //@}

    //! @name Operations 
    //@{
    void Update    ( bool bIsDead );
    void Clean     ();
    bool HasChanged() const;
    //@}

    //! @name Dotations management
    //@{
    virtual bool     HasDotation        ( const PHY_DotationCategory& category ) const;
            MT_Float GetDotationCapacity( const PHY_DotationCategory& category ) const;
            MT_Float GetDotationValue   ( const PHY_DotationCategory& category ) const;
            MT_Float ConsumeDotation    ( const PHY_DotationCategory& category, MT_Float rNbr );
            MT_Float SupplyDotation     ( const PHY_DotationCategory& category, MT_Float rNbr );
            void     ResupplyDotations  ();
            void     ResupplyDotations  ( const PHY_AmmoDotationClass& ammoDotationClass, MT_Float rFactor );
            void     ResupplyDotations  ( const PHY_DotationType&      type             , MT_Float rFactor );
    //@}

    //! @name Consumption management
    //@{
                  bool                 SetConsumptionMode      ( const PHY_ConsumptionType& mode );
    virtual const PHY_ConsumptionType& GetConsumptionMode      () const;
                  void                 RollbackConsumptionMode ();
                  MT_Float             GetMaxTimeForConsumption( const PHY_ConsumptionType& mode ) const;
    //@}

    //! @name Fire dotations
    //@{
    MT_Float AddFireReservation( const PHY_DotationCategory& category, MT_Float rNbr );
    //@}

    //! @name Network
    //@{
    void SendChangedState( NET_ASN_MsgUnitAttributes& asn ) const;
    void SendFullState   ( NET_ASN_MsgUnitAttributes& asn ) const;
    //@}

    //! @name Logistic - Supply
    //@{
    void NotifySupplyNeeded          ( const PHY_DotationCategory& dotationCategory, bool bNewNeed ) const; // Logistic
    void FillSupplyRequest           ( PHY_SupplyDotationRequestContainer& supplyRequest ) const;
    void ChangeDotationsValueUsingTC2( const PHY_DotationType& dotationType, const PHY_AmmoDotationClass* pAmmoDotationClass, MT_Float rCapacityFactor ) const;
    //@}

public:
    //! @name Types
    //@{
    typedef std::map< const PHY_Dotation*, MT_Float /*rValueReserved*/ > T_DotationReservedMap;
    typedef T_DotationReservedMap::const_iterator                        CIT_DotationReservedMap;
    //@}

private:
          MIL_AgentPion*              pPion_;
          PHY_DotationGroupContainer* pDotations_;

    const PHY_ConsumptionType*  pCurrentConsumptionMode_;
    const PHY_ConsumptionType*  pPreviousConsumptionMode_;
          T_DotationReservedMap reservedConsumptions_;

	template< typename Archive > friend  void save_construct_data( Archive& archive, const PHY_RolePion_Dotations* role, const unsigned int /*version*/ );
	template< typename Archive > friend  void load_construct_data( Archive& archive, PHY_RolePion_Dotations* role, const unsigned int /*version*/ );
};

#endif // __PHY_RolePion_Dotations_h_
