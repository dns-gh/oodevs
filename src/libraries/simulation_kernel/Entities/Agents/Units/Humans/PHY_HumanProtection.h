// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/Src/Entities/Agents/Units/Categories/PHY_HumanProtection.h $
// $Author: Nld $
// $Modtime: 11/03/05 14:04 $
// $Revision: 5 $
// $Workfile: PHY_HumanProtection.h $
//
// *****************************************************************************

#ifndef __PHY_HumanProtection_h_
#define __PHY_HumanProtection_h_

#include "MIL_Random.h"
#include "MT_Tools/MT_String.h"

namespace xml
{
    class xistream;
}

class PHY_ComposanteState;

// =============================================================================
// @class  PHY_HumanProtection
// Created: JVT 2004-08-03
// =============================================================================
class PHY_HumanProtection : private boost::noncopyable
{
private:
    //! @name Types
    //@{
    typedef std::map< std::string , const PHY_HumanProtection* > T_HumanProtectionMap;
    typedef T_HumanProtectionMap::iterator                      IT_HumanProtectionMap;
    typedef T_HumanProtectionMap::const_iterator               CIT_HumanProtectionMap;

    typedef std::list< std::string >        T_InjuryTypes;
    typedef T_InjuryTypes::iterator        IT_InjuryTypes;
    typedef T_InjuryTypes::const_iterator CIT_InjuryTypes;


    typedef std::list< std::pair< float, float > > T_InjuryEffects;
    typedef T_InjuryEffects::iterator             IT_InjuryEffects;
    typedef T_InjuryEffects::const_iterator      CIT_InjuryEffects;

    struct ProtectionDescription
    {
        ProtectionDescription() {}
        std::string injuryName_;
        T_InjuryTypes injuryTypes_;
        T_InjuryEffects injuryEffects_;
    };

    typedef std::list< PHY_HumanProtection::ProtectionDescription > T_ProtectionList;
    typedef T_ProtectionList::iterator                             IT_ProtectionList;
    typedef T_ProtectionList::const_iterator                      CIT_ProtectionList;
    //@}

public:
    //! @name Manager
    //@{
    static void Initialize( xml::xistream& xis );
    static void Terminate ();

    static const T_HumanProtectionMap& GetProtections();
    static const PHY_HumanProtection* Find( const std::string& strName );

    PHY_HumanProtection& InstanciateHumanProtection() const;
    //@}

    //! @name Accessors
    //@{
    const std::string GetName() const;
    float ComputeProtectionValue( int injuryID, int threshold, const std::string type ) const;
    //@}

private:
    bool IsProtectionAgainstThisType( const std::string type, T_InjuryTypes injuryTypesList ) const;

public:
    //! @name Constructor/Destructor
    //@{
             PHY_HumanProtection( const std::string& strName, xml::xistream& xis );
             PHY_HumanProtection( const PHY_HumanProtection& humanProtection );
    virtual ~PHY_HumanProtection();
    //@}

private:
    //! @name Helpers
    //@{
    static void ReadProtection( xml::xistream& xis );
    void ReadHumanProtection( xml::xistream& xis );
    void ReadType( xml::xistream& xis );
    void ReadEffect( xml::xistream& xis );
    //@}

private:
    //! @name Member data
    //@{
    const std::string strName_;
    unsigned int nID_;
    ProtectionDescription protectionDescription_;
    //@}

private:
    //! @name static members
    static T_HumanProtectionMap protections_;
    static unsigned int nNextID_;
    //@}
};

#endif // __PHY_HumanProtection_h_
