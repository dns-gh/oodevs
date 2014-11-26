// *****************************************************************************
//
// $Created: NLD 2004-03-31 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Decision/Functions/DEC_FireFunctions.h $
// $Author: Jvt $
// $Modtime: 2/05/05 11:39 $
// $Revision: 4 $
// $Workfile: DEC_FireFunctions.h $
//
// *****************************************************************************

#ifndef __DEC_FireFunctions_h_
#define __DEC_FireFunctions_h_

namespace boost
{
    template< typename T > class shared_ptr;
}

namespace sword
{
    class Brain;
}

class DEC_Knowledge_Agent;
class DEC_Decision_ABC;
class PHY_DotationCategory;

// =============================================================================
// Created: NLD 2004-03-31
// =============================================================================
class DEC_FireFunctions
{
public:
    static void Register( sword::Brain& brain );

    //! @name Functions
    //@{
    static float GetMaxRangeToFireOnEnemy( const DEC_Decision_ABC* callerAgent, boost::shared_ptr< DEC_Knowledge_Agent > pKnowledge, float rWantedPH );
    static float GetMaxRangeToFireOnEnemyWithDotation( const DEC_Decision_ABC* callerAgent, boost::shared_ptr< DEC_Knowledge_Agent > pKnowledge, float rWantedPH, const PHY_DotationCategory* dotation );
    static float GetMinRangeToFireOnEnemy( const DEC_Decision_ABC* callerAgent, boost::shared_ptr< DEC_Knowledge_Agent > pKnowledge, float rWantedPH );
    static float GetMaxRangeToFireOnEnemyActualPosture( const DEC_Decision_ABC* callerAgent, boost::shared_ptr< DEC_Knowledge_Agent > pKnowledge, float rWantedPH );
    static float GetMinRangeToFireOnEnemyActualPosture( const DEC_Decision_ABC* callerAgent, boost::shared_ptr< DEC_Knowledge_Agent > pKnowledge, float rWantedPH );
    static float GetMaxRangeToFireOnEnemyWhenUnloaded( const DEC_Decision_ABC* callerAgent, boost::shared_ptr< DEC_Knowledge_Agent > pKnowledge, float rWantedPH );
    static float GetMaxRangeToBeFiredByEnemy( const DEC_Decision_ABC* callerAgent, boost::shared_ptr< DEC_Knowledge_Agent > pKnowledge, float rWantedPH );
    static float GetMaxRangeToFire( const DEC_Decision_ABC* callerAgent, float rWantedPH );
    static float GetMaxRangeToIndirectFire( const DEC_Decision_ABC* callerAgent, const PHY_DotationCategory* pDotationCategory );
    static float GetMaxRangeToIndirectFire( const DEC_Decision_ABC* callerAgent );
    static float GetMinRangeToIndirectFire( const DEC_Decision_ABC* callerAgent, const PHY_DotationCategory* pDotationCategory );
    static float GetTheoricMaxRangeToIndirectFire( const DEC_Decision_ABC* callerAgent, const PHY_DotationCategory* pDotationCategory );
    static float GetTheoricMinRangeToIndirectFire( const DEC_Decision_ABC* callerAgent, const PHY_DotationCategory* pDotationCategory );
    static float GetMaxRangeToIndirectFireWithoutAmmoCheck( const DEC_Decision_ABC* callerAgent, const PHY_DotationCategory* pDotationCategory  );
    static float GetMinRangeToIndirectFireWithoutAmmoCheck( const DEC_Decision_ABC* callerAgent, const PHY_DotationCategory* pDotationCategory  );
    static double ThrowSmokeOnKnowledgeAgent( DEC_Decision_ABC* callerAgent, boost::shared_ptr< DEC_Knowledge_Agent > pTarget );
    static double ThrowSmokeOnPosition( const DEC_Decision_ABC& callerAgent, boost::shared_ptr< MT_Vector2D > pTarget );
    static const PHY_DotationCategory* GetMunitionForIndirectFire( DEC_Decision_ABC* callerAgent, int indirectFireDotationClassID, const MT_Vector2D* pTarget );
    static void ForbidAmmunition( DEC_Decision_ABC* callerAgent, const std::vector< const PHY_DotationCategory* >& dotationCategories );
    static void AllowAmmunition( DEC_Decision_ABC* callerAgent, const std::vector< const PHY_DotationCategory* >& dotationCategories );
    static void AllowAllAmmunitions( DEC_Decision_ABC* callerAgent );
    //@}
};

#endif // __DEC_FireFunctions_h_
