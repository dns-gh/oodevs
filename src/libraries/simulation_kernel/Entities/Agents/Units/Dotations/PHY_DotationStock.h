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

class PHY_DotationCategory;
class PHY_DotationStockContainer;

// =============================================================================
// @class  PHY_DotationStock
// Created: JVT 2004-08-03
// =============================================================================
class PHY_DotationStock : private boost::noncopyable
{
public:
    //! @name Constructors/Destructor
    //@{
     PHY_DotationStock( PHY_DotationStockContainer& stockContainer, const PHY_DotationCategory& dotationCategory, double rSupplyThresholdRatio, double rCapacity, bool bInfiniteDotations, bool bCreateEmpty = false );
     PHY_DotationStock();
    virtual ~PHY_DotationStock();
    //@}

    //! @name CheckPoints
    //@{
    BOOST_SERIALIZATION_SPLIT_MEMBER()

    void load( MIL_CheckPointInArchive&, const unsigned int );
    void save( MIL_CheckPointOutArchive&, const unsigned int ) const;
    //@}

    //! @name Accessors
    //@{
    const PHY_DotationCategory& GetCategory() const;
          double              GetValue   () const;
          double              GetCapacity() const;
          bool                IsFull() const;
          bool                IsEmpty() const;
    //@}

    //! @name Operations
    //@{
    double AddReservation   ( double rNbr );
    void     RemoveReservation( double rNbr );
    void     Resupply        ();
    //@}

    //! @name Log Supply
    //@{
    bool     HasReachedSupplyThreshold() const;
    bool     NeedSupply               () const;
    double Supply                   ( double rSupply );
    //@}
    
    //! @name Used for stock extraction
    //@{
    double Consume( double rValue );
    double SupplyUntilFull( double rSupply );
    //@}

private:
    //! @name Tools
    //@{
    void     SetValue  ( double rValue );
    //@}

private:
    //! @name Member data
    //@{
    static const double             maxCapacity_;
          PHY_DotationStockContainer* pStockContainer_;
    const PHY_DotationCategory*       pCategory_;
          double                    rValue_;
          double                    rCapacity_; // Les stocks peuvent d�passer leurs capacit�s (Stockage � terre)
          double                    rSupplyThreshold_;
          bool                        bInfiniteDotations_;
    //@}
};

BOOST_CLASS_EXPORT_KEY( PHY_DotationStock )

#endif // __PHY_DotationStock_h_
