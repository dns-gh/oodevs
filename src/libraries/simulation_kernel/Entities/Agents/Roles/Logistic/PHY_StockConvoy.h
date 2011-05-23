// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Agents/Roles/Logistic/PHY_StockConvoy.h $
// $Author: Nld $
// $Modtime: 14/04/05 10:10 $
// $Revision: 5 $
// $Workfile: PHY_StockConvoy.h $
//
// *****************************************************************************

#ifndef __PHY_StockConvoy_h_
#define __PHY_StockConvoy_h_

#include "MIL.h"
#include "PHY_Convoy_ABC.h"

namespace xml
{
    class xistream;
}

class PHY_SupplyStockConsign;
class MIL_AgentPion;

// =============================================================================
// @class  PHY_StockConvoy
// Created: JVT 2004-08-03
// =============================================================================
class PHY_StockConvoy : public PHY_Convoy_ABC
{
public:
    //! @name
    //@{
    static void Initialize( xml::xistream& xis );
    static void Terminate ();
    //@}

public:
    explicit PHY_StockConvoy( PHY_SupplyStockConsign& consign );
             PHY_StockConvoy();
    virtual ~PHY_StockConvoy();

    //! @name CheckPoints
    //@{
    BOOST_SERIALIZATION_SPLIT_MEMBER()

    void load( MIL_CheckPointInArchive&, const unsigned int );
    void save( MIL_CheckPointOutArchive&, const unsigned int ) const;
    //@}

    //! @name Operations
    //@{
    bool Form();
    //@}

    //! @name 'Real' convoy operations
    //@{
    const MIL_AgentPion* GetPionConvoy() const;
    void ActivateConvoyMission();
    void DesactivateConvoyMission();
    //@}

    //! @name Script operations
    //@{
    bool Load           ();
    bool Unload         ();
    void EndMission     ();
    bool IsLoadingDone  () const;
    bool IsUnloadingDone() const;
    //@}

private:
    //! @name Member data
    //@{
    PHY_SupplyStockConsign* pConsign_;
    MIL_AgentPion*          pPionConvoy_;
    bool                    bMissionActivated_;
    //@}
};

BOOST_CLASS_EXPORT_KEY( PHY_StockConvoy )

#endif // __PHY_StockConvoy_h_
