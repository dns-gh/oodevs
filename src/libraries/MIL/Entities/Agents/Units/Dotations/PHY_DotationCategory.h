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

#include "MIL.h"

#include "Entities/Agents/Units/Weapons/PHY_AttritionData.h"

class PHY_DotationType;
class PHY_DotationCategory_IndirectFire_ABC;
class PHY_AmmoDotationClass;
class PHY_Protection;
class PHY_FireResults_ABC;
class MIL_AgentPion;

// =============================================================================
// @class  PHY_DotationCategory
// Created: JVT 2004-08-03
// =============================================================================
class PHY_DotationCategory
{
    MT_COPYNOTALLOWED( PHY_DotationCategory )

public:
     PHY_DotationCategory( const PHY_DotationType& type, const std::string& strName, MIL_InputArchive& archive );
    ~PHY_DotationCategory();

    //! @name Accessors
    //@{
          uint                   GetMosID            () const;
    const PHY_DotationType&      GetType             () const;
    const std::string&           GetName             () const;
    const PHY_AmmoDotationClass& GetAmmoDotationClass() const;
    //@}

    //! @name Fire
    //@{
    bool CanBeUsedForDirectFire  () const;
    bool CanBeUsedForIndirectFire() const;
    bool HasAttritions           () const;

    const PHY_AttritionData&                     GetAttritionData   ( const PHY_Protection& protectionTarget ) const;
          MT_Float                               GetAttritionScore  ( const PHY_Protection& protectionTarget ) const;
    const PHY_DotationCategory_IndirectFire_ABC* GetIndirectFireData() const;

    void IndirectFire( const MIL_AgentPion& firer, const MT_Vector2D& vSourcePosition, const MT_Vector2D& vTargetPosition, uint nNbrAmmoFired, PHY_FireResults_ABC& fireResult ) const;
    void IndirectFire( const MIL_AgentPion& firer, const MT_Vector2D& vSourcePosition, const MT_Vector2D& vTargetPosition, uint nNbrAmmoFired ) const;
    //@}

    //! @name Tools
    //@{
    MT_Float ConvertUVolumeToDotation( MT_Float rValue ) const;
    MT_Float ConvertDotationToUVolume( MT_Float rValue ) const;
    //@}

    //! @name Operators
    //@{
    bool operator==( const PHY_DotationCategory& rhs ) const;
    bool operator!=( const PHY_DotationCategory& rhs ) const;
    //@}

private:
    //! @name Types
    //@{
    typedef std::vector< PHY_AttritionData >  T_AttritionVector;
    typedef T_AttritionVector                 CIT_AttritionVector;
    //@}

private:
    //! @name Init
    //@{
    void InitializeAttritions      ( MIL_InputArchive& archive );
    void InitializeIndirectFireData( MIL_InputArchive& archive );
    //@}

private:
    const PHY_DotationType&      type_;
    const PHY_AmmoDotationClass* pAmmoDotationClass_;      

    std::string strName_;
    uint        nMosID_;
    MT_Float    rUVolume_;

    T_AttritionVector                      attritions_;
    PHY_DotationCategory_IndirectFire_ABC* pIndirectFireData_;     
};

#include "PHY_DotationCategory.inl"

#endif // __PHY_DotationCategory_h_
