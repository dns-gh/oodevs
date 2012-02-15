// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/Src/Entities/Agents/Units/Categories/PHY_Protection.h $
// $Author: Nld $
// $Modtime: 11/03/05 14:04 $
// $Revision: 5 $
// $Workfile: PHY_Protection.h $
//
// *****************************************************************************

#ifndef __PHY_Protection_h_
#define __PHY_Protection_h_

#include "MT_Tools/MT_GaussianRandom.h"

namespace xml
{
    class xistream;
}

class PHY_ComposanteState;

// =============================================================================
// @class  PHY_Protection
// Created: JVT 2004-08-03
// =============================================================================
class PHY_Protection : private boost::noncopyable
{
public:
    //! @name Types
    //@{
    typedef std::map< std::string, const PHY_Protection* > T_ProtectionMap;
    typedef T_ProtectionMap::const_iterator              CIT_ProtectionMap;
    //@}

public:
    //! @name Manager
    //@{
    static void Initialize( xml::xistream& xis );
    static void Terminate();

    static const T_ProtectionMap& GetProtections();
    static const PHY_Protection* Find( const std::string& strName );
    //@}

    //! @name Accessors
    //@{
    const std::string& GetName() const;
    unsigned int GetID() const;
    unsigned int GetNeutralizationTime() const;
    double GetHumanDeadRatio( const PHY_ComposanteState& composanteState ) const;
    double GetHumanWoundedRatio( const PHY_ComposanteState& composanteState ) const;
    bool IsHuman() const;
    bool IsCrowd() const;
    //@}

    //! @name Operations
    //@{
    bool TriggerRandomEvacuationBreakdown () const;
    bool TriggerRandomNonEvacuationBreakdown() const;
    //@}

private:
    //! @name Constructor/Destructor
    //@{
             PHY_Protection( const std::string& strName, xml::xistream& xis );
    virtual ~PHY_Protection();
    //@}

    //! @name Helpers
    //@{
    static void ReadProtection( xml::xistream& xis );
    void ReadAttrition ( xml::xistream& xis );
    //@}

private:
    //! @name Types
    //@{
    enum E_Type
    {
        eHuman,
        eMaterial,
        eCrowd
    };

    struct T_HumanEffect
    {
        T_HumanEffect();

        double rDeadRatio_;
        double rWoundedRatio_;
    };
    typedef std::vector< T_HumanEffect >    T_HumanEffects;
    typedef T_HumanEffects::const_iterator IT_HumanEffects;
    //@}

private:
    //! @name Member data
    //@{
    const std::string strName_;
    unsigned int nID_;
    E_Type nType_;
    T_HumanEffects attritionEffectsOnHumans_;
    double rBreakdownProbabilityEva_;
    double rBreakdownProbabilityNeva_;
    mutable MT_GaussianRandom neutralizationTime_;
    static T_ProtectionMap protections_;
    static unsigned int nNextID_;
    //@}
};

#endif // __PHY_Protection_h_
