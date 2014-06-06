// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/Src/Entities/Agents/Units/Dotations/PHY_DotationCategory.h $
// $Author: Nld $
// $Modtime: 17/03/05 15:15 $
// $Revision: 6 $
// $Workfile: PHY_DotationCategory.h $
//
// *****************************************************************************

#ifndef __PHY_DotationCategory_h_
#define __PHY_DotationCategory_h_

#include "Entities/Agents/Units/Weapons/PHY_AttritionData.h"

class MIL_Agent_ABC;
class PHY_DotationType;
class PHY_DotationNature;
class PHY_DotationCategory_IndirectFire_ABC;
class PHY_IndirectFireDotationClass;
class PHY_AmmoDotationClass;
class PHY_Protection;
class PHY_DotationLogisticType;
class PHY_MaterialCompositionType;
class PHY_UrbanAttritionData;
class PHY_FireResults_ABC;
class MT_Vector2D;

// =============================================================================
// @class  PHY_DotationCategory
// Created: JVT 2004-08-03
// =============================================================================
class PHY_DotationCategory : private boost::noncopyable
{
private:
    typedef std::vector< PHY_DotationCategory_IndirectFire_ABC* > T_IndirectFireEffects;

public:
             PHY_DotationCategory( const PHY_DotationType& type, const std::string& strName, xml::xistream& xis );
    virtual ~PHY_DotationCategory();

    //! @name Accessors
    //@{
    unsigned int GetMosID() const;
    const PHY_DotationType& GetType() const;
    const std::string& GetName() const;
    const PHY_AmmoDotationClass* GetAmmoDotationClass() const;
    const PHY_DotationLogisticType& GetLogisticType() const;
    const PHY_DotationNature& GetNature() const;
    bool IsGuided() const;
    float GetGuidanceRange() const;
    bool IsIlluminating( float range, bool permanent ) const;
    float GetIlluminatingRange() const;
    const T_IndirectFireEffects& GetIndirectFireEffects() const;
    bool IsIED() const;
    //@}

    //! @name Fire
    //@{
    bool CanBeUsedForDirectFire  () const;
    bool CanBeUsedForIndirectFire() const;
    bool HasAttritions           () const;
    bool HasIndirectWeaponCategory( const PHY_IndirectFireDotationClass& indirectWeaponCategory ) const;
    double GetSmokeDuration() const;

    const PHY_AttritionData& GetAttritionData( const PHY_Protection& protectionTarget ) const;
    double GetAttritionScore( const PHY_Protection& protectionTarget ) const;

    void ApplyIndirectFireEffect( const MT_Vector2D& vSourcePosition, const MT_Vector2D& vTargetPosition, unsigned int nNbrAmmoFired, PHY_FireResults_ABC& fireResult ) const;
    void ApplyIndirectFireEffect( const MIL_Agent_ABC& firer, const MT_Vector2D& vSourcePosition, const MT_Vector2D& vTargetPosition, unsigned int nNbrAmmoFired, PHY_FireResults_ABC& fireResult ) const;
    void ApplyStrikeEffect( const MIL_Agent_ABC& firer, MIL_Agent_ABC& target, unsigned int nNbrAmmoFired, PHY_FireResults_ABC& fireResult ) const;

    double ConvertToNbrAmmo( double rNbrIT ) const;
    double ConvertToInterventionType( unsigned int nNbr ) const;
    double GetRadius() const;
    //@}

    //! @name Packaging
    //@{
    double GetWeight() const;
    double GetVolume() const;
    //@}

    //! @name Operators
    //@{
    bool operator==( const PHY_DotationCategory& rhs ) const;
    bool operator!=( const PHY_DotationCategory& rhs ) const;
    //@}

    //! @name 
    //@{
           double GetUrbanAttritionScore( const PHY_MaterialCompositionType& material ) const;
    static double FindUrbanAttritionScore( const PHY_MaterialCompositionType& material ); // Get global score
    //@}
    
    //! @name 
    //@{
    bool IsSignificantChange( double oldValue, double newValue, double capacity ) const;
    //@}
    

private:
    //! @name Types
    //@{
    typedef std::vector< PHY_AttritionData > T_AttritionVector;
    typedef std::vector< double > T_UrbanAttritionVector;
    //@}

private:
    //! @name Init
    //@{
    void InitializeAttritions      ( xml::xistream& xis );
    void InitializeUrbanAttritions ( xml::xistream& xis );
    void InitializeIndirectFireData( xml::xistream& xis );
    void InitializePackagingData   ( xml::xistream& xis );
    void InitializeLogisticType    ( xml::xistream& xis );
    void InitializeIllumination    ( xml::xistream& xis );
    void InitializeGuidance        ( xml::xistream& xis );
    //@}
    //! @name Helpers
    //@{
    void ListAttrition             ( xml::xistream& xis );
    void ReadAttrition             ( xml::xistream& xis );
    void ListUrbanAttrition        ( xml::xistream& xis );
    void ReadUrbanAttritionModifier( xml::xistream& xis );
    void ReadIndirectFire          ( xml::xistream& xis, unsigned int nInterventionType, double rDispersionX, double rDispersionY, double rDetectionRange );
    //@}

private:
    const PHY_DotationType& type_;
    const PHY_AmmoDotationClass* pAmmoDotationClass_;
    const PHY_DotationLogisticType* pLogisticType_;
    const PHY_DotationNature* pNature_;

    std::string strName_;
    unsigned int nMosID_;

    // Packaging
    double rWeight_;
    double rVolume_;

    T_AttritionVector attritions_;
    std::unique_ptr< PHY_UrbanAttritionData > urbanAttritionData_;
    T_IndirectFireEffects indirectFireEffects_;

    //Illumination capacity
    float fRange_;
    bool  bIlluminating_;
    bool  bMaintainIllumination_;

    bool ied_;
    //Guidance
    bool bGuided_;
    bool bMaintainGuidance_;
    float rGuidanceRange_;
    unsigned int nInterventionType_;
    double rDispersionX_;
    double rDispersionY_;
    double rDetectionRange_;

private:
    static unsigned int nbComposantes;
};

namespace boost
{
namespace archive
{
    template< class Archive >
    void save( Archive& ar, const PHY_DotationCategory* t )
    {
        unsigned int id = t ? t->GetMosID() : std::numeric_limits< unsigned int >::max();
        ar << id;
    }
    template< class Archive >
    void load( Archive& ar, const PHY_DotationCategory*& t )
    {
        unsigned int id;
        ar >> id;
        t = PHY_DotationType::FindDotationCategory( id );
    }
}
}

#endif // __PHY_DotationCategory_h_
