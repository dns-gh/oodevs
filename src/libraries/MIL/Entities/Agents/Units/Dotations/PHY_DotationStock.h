// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Agents/Units/Dotations/PHY_DotationStock.h $
// $Author: Jvt $
// $Modtime: 1/04/05 13:24 $
// $Revision: 3 $
// $Workfile: PHY_DotationStock.h $
//
// *****************************************************************************

#ifndef __PHY_DotationStock_h_
#define __PHY_DotationStock_h_

#include "MIL.h"

class PHY_DotationCapacity;
class PHY_DotationCategory;
class PHY_DotationStockContainer;

// =============================================================================
// @class  PHY_DotationStock
// Created: JVT 2004-08-03
// =============================================================================
class PHY_DotationStock
{
    MT_COPYNOTALLOWED( PHY_DotationStock )

public:
     PHY_DotationStock( PHY_DotationStockContainer& stockContainer, const PHY_DotationCategory& dotationCategory, MT_Float rSupplyThresholdRatio, MT_Float rCapacity );
     PHY_DotationStock();
    ~PHY_DotationStock();

    //! @name CheckPoints
    //@{
    BOOST_SERIALIZATION_SPLIT_MEMBER()
    
    void load( MIL_CheckPointInArchive&, const uint );
    void save( MIL_CheckPointOutArchive&, const uint ) const;
    //@}

    //! @name Accessors
    //@{
    const PHY_DotationCategory& GetCategory() const;
          MT_Float              GetValue   () const;
          MT_Float              GetCapacity() const;
    //@}

    //! @name Operations
    //@{
    MT_Float AddReservation   ( MT_Float rNbr );
    void     RemoveReservation( MT_Float rNbr );
    void     Resupply        ();
    //@}

    //! @name Log Supply
    //@{
    bool     HasReachedSupplyThreshold() const;
    bool     NeedSupply               () const;
    MT_Float Supply                   ( MT_Float rSupply );
    //@}

private:
    //! @name Tools
    //@{
    void     SetValue  ( MT_Float rValue );
    MT_Float Consume   ( MT_Float rValue );
    //@}

private:
          PHY_DotationStockContainer* pStockContainer_;
    const PHY_DotationCategory*       pCategory_;
          MT_Float                    rValue_;
          MT_Float                    rCapacity_; // Les stocks peuvent dépasser leurs capacités (Stockage à terre)
          MT_Float                    rSupplyThreshold_;    
};

#include "PHY_DotationStock.inl"

#endif // __PHY_DotationStock_h_
