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

namespace xml
{
    class xistream;
}

class MIL_Agent_ABC;
class MIL_EntityManager_ABC;
class PHY_DotationType;
class PHY_DotationNature;
class PHY_DotationCategory_IndirectFire_ABC;
class PHY_AmmoDotationClass;
class PHY_Protection;
class PHY_DotationLogisticType;
class PHY_FireResults_ABC;

// =============================================================================
// @class  PHY_DotationCategory
// Created: JVT 2004-08-03
// =============================================================================
class PHY_DotationCategory : private boost::noncopyable
{

public:
     PHY_DotationCategory( const PHY_DotationType& type, const std::string& strName, xml::xistream& xis );
    virtual ~PHY_DotationCategory();

    //! @name Accessors
    //@{
          unsigned int                      GetMosID            () const;
    const PHY_DotationType&         GetType             () const;
    const std::string&              GetName             () const;
    const PHY_AmmoDotationClass*    GetAmmoDotationClass() const;
    const PHY_DotationLogisticType& GetLogisticType     () const;
    const PHY_DotationNature&       GetNature           () const;
          bool                      IsGuided            () const;
          bool                      IsIlluminating      ( float range, bool permanent ) const;
    //@}

    //! @name Fire
    //@{
    bool CanBeUsedForDirectFire  () const;
    bool CanBeUsedForIndirectFire() const;
    bool HasAttritions           () const;

    const PHY_AttritionData&                     GetAttritionData   ( const PHY_Protection& protectionTarget ) const;
          MT_Float                               GetAttritionScore  ( const PHY_Protection& protectionTarget ) const;
    const PHY_DotationCategory_IndirectFire_ABC* GetIndirectFireData() const;

    void ApplyIndirectFireEffect( const MIL_Agent_ABC& firer, const MT_Vector2D& vSourcePosition, const MT_Vector2D& vTargetPosition, unsigned int nNbrAmmoFired, PHY_FireResults_ABC& fireResult ) const;
    void ApplyIndirectFireEffect( const MIL_Agent_ABC& firer, MIL_Agent_ABC& target, unsigned int nNbrAmmoFired, PHY_FireResults_ABC& fireResult ) const;
    //@}

    //! @name Packaging
    //@{
    MT_Float GetWeight() const;
    MT_Float GetVolume() const;
    //@}

    //! @name Operators
    //@{
    bool operator==( const PHY_DotationCategory& rhs ) const;
    bool operator!=( const PHY_DotationCategory& rhs ) const;
    //@}

private:
    //! @name Types
    //@{
    typedef std::vector< PHY_AttritionData > T_AttritionVector;
    //@}

private:
    //! @name Init
    //@{
    void InitializeAttritions      ( xml::xistream& xis );
    void InitializeIndirectFireData( xml::xistream& xis );
    void InitializePackagingData   ( xml::xistream& xis );
    void InitializeLogisticType    ( xml::xistream& xis );
    void InitializeIllumination    ( xml::xistream& xis );
    void InitializeGuidance        ( xml::xistream& xis );
    //@}
    //! @name Helpers
    //@{
    void ListAttrition   ( xml::xistream& xis );
    void ReadAttrition   ( xml::xistream& xis );
    void ReadIndirectFire( xml::xistream& xis );
    //@}

private:
    const PHY_DotationType&         type_;
    const PHY_AmmoDotationClass*    pAmmoDotationClass_;      
    const PHY_DotationLogisticType* pLogisticType_;
    const PHY_DotationNature*       pNature_;

    std::string strName_;
    unsigned int        nMosID_;
        
    // Packaging
    MT_Float rWeight_;
    MT_Float rVolume_;

    T_AttritionVector                      attritions_;
    PHY_DotationCategory_IndirectFire_ABC* pIndirectFireData_;

    //Illumination capacity
    bool  bIlluminating_;
    float fRange_;
    bool  bMaintainIllumination_;
    //Guidance
    bool bGuided_;
    bool bMaintainGuidance_;

};

#endif // __PHY_DotationCategory_h_
