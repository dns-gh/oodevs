// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Agents/Units/Weapons/PHY_UrbanAttritionData.h $
// $Author: Nld $
// $Modtime: 19/10/04 13:54 $
// $Revision: 1 $
// $Workfile: PHY_UrbanAttritionData.h $
//
// *****************************************************************************

#ifndef __PHY_UrbanAttritionData_h_
#define __PHY_UrbanAttritionData_h_

namespace xml
{
    class xistream;
}

class PHY_UrbanAttritionData;
class PHY_MaterialCompositionType;

// =============================================================================
// @class  PHY_UrbanAttritionData
// Created: JVT 2004-08-03
// =============================================================================
class PHY_UrbanAttritionData
{
public:
    explicit PHY_UrbanAttritionData( xml::xistream& xis );
             PHY_UrbanAttritionData( const PHY_UrbanAttritionData& data );
    virtual ~PHY_UrbanAttritionData();

    //! @name Global Parameters
    //@{
    void UpdateGlobalScore();
    static double GetGlobalScore( const PHY_MaterialCompositionType& material );
    static void Init();
    //@}

    //! @name Operations
    //@{
    double GetScore( const PHY_MaterialCompositionType& material ) const;
    //@}

private:
    //! @name Helpers
    //@{
    void ReadModifier( xml::xistream& xis );
    //@}

private:
    //! @name Types
    //@{
    typedef std::vector< double >   T_UrbanAttritionVector;
    //@}

private:
    T_UrbanAttritionVector attritionFactors_;

private:
    static T_UrbanAttritionVector globalAttritionFactors_;
};

#endif // __PHY_UrbanAttritionData_h_
