// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/Src/Entities/Agents/Units/Dotations/PHY_DotationLogisticType.h $
// $Author: Nld $
// $Modtime: 17/03/05 17:26 $
// $Revision: 8 $
// $Workfile: PHY_DotationLogisticType.h $
//
// *****************************************************************************

#ifndef __PHY_DotationLogisticType_h_
#define __PHY_DotationLogisticType_h_

#include "MIL.h"

class PHY_DotationCategory;

#include "MT_Tools/MT_Converter.h"

// =============================================================================
// @class  PHY_DotationLogisticType
// Created: JVT 2004-08-03
// =============================================================================
class PHY_DotationLogisticType
{
    MT_COPYNOTALLOWED( PHY_DotationLogisticType )

public:
    //! @name Types
    //@{
    typedef std::map< std::string, const PHY_DotationLogisticType*, sCaseInsensitiveLess > T_DotationLogisticTypeMap;
    typedef T_DotationLogisticTypeMap::const_iterator                                      CIT_DotationLogisticTypeMap;
    //@}

    //! @name Statics
    //@{
    static PHY_DotationLogisticType uniteEssence_;
    static PHY_DotationLogisticType uniteFeuTD_;
    static PHY_DotationLogisticType uniteFeuSansTD_;
    static PHY_DotationLogisticType uniteVivre_;
    static PHY_DotationLogisticType pieces_;
    //@}

public:
    //! @name Manager
    //@{
    static void Initialize();
    static void Terminate ();

    static const PHY_DotationLogisticType*  Find                    ( const std::string& strName );
    static const PHY_DotationLogisticType*  Find                    ( uint nID );
    static const T_DotationLogisticTypeMap& GetDotationLogisticTypes();
    //@}

    //! @name Accessors
    //@{
    const std::string& GetName() const;
          uint         GetID  () const;
    //@}

    //! @name Operators
    //@{
//    bool operator==( const PHY_DotationLogisticType& rhs ) const;
    //@}

private:
    //! @name Types
    //@{
    enum E_DotationLogisticType
    {
        eUniteEssence   = 0,
        eUniteFeuTD     = 1,
        eUniteFeuSansTD = 2,
        eUniteVivre     = 3,        
        ePieces         = 4
    };
    //@}

private:
     PHY_DotationLogisticType( const std::string& strName, E_DotationLogisticType nType );
    ~PHY_DotationLogisticType();

private:
    const std::string            strName_;
    const E_DotationLogisticType nType_;

private:
    static T_DotationLogisticTypeMap dotationLogisticTypes_;
};

#include "PHY_DotationLogisticType.inl"

#endif // __PHY_DotationLogisticType_h_
