// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Agents/Units/Weapons/PHY_LauncherType.h $
// $Author: Nld $
// $Modtime: 19/10/04 13:54 $
// $Revision: 1 $
// $Workfile: PHY_LauncherType.h $
//
// *****************************************************************************

#ifndef __PHY_LauncherType_h_
#define __PHY_LauncherType_h_

#include "MT_Tools/MT_String.h"

namespace xml
{
    class xistream;
}

class PHY_Posture;
class PHY_RoleInterface_Posture;

// =============================================================================
// @class  PHY_LauncherType
// Created: JVT 2004-08-03
// =============================================================================
class PHY_LauncherType : private boost::noncopyable
{
public:
    //! @name Manager
    //@{
    static void Initialize( xml::xistream& xis );
    static void Terminate ();

    static const PHY_LauncherType* FindLauncherType( const std::string& strName );
    //@}

    //! @name Operations
    //@{
    double GetPHModificator( const PHY_RoleInterface_Posture& firerPosture, const PHY_RoleInterface_Posture& targetPosture ) const;
    double GetPHModificator( const PHY_Posture&               firerPosture, const PHY_Posture&               targetPosture ) const;
    //@}

    //! @name Accessors
    //@{
    const std::string& GetName        () const;
          bool         CanDirectFire  () const;
          bool         CanIndirectFire() const;
    //@}

private:
    //! @name Types
    //@{
    typedef std::map< std::string, const PHY_LauncherType* > T_LauncherTypeMap;
    typedef T_LauncherTypeMap::const_iterator CIT_LauncherTypeMap;

    typedef std::vector< double > T_PhModificatorValueVector;
    typedef std::vector< T_PhModificatorValueVector > T_PhModificatorsVector;
    typedef T_PhModificatorsVector::const_iterator CIT_PhModificatorsVector;
    //@}

private:
     PHY_LauncherType( const std::string& strName, xml::xistream& xis );
    ~PHY_LauncherType();

    //! @name Init
    //@{
    void InitializeForIndirectFire( xml::xistream& xis );
    void InitializeForDirectFire  ( xml::xistream& xis );
    void RegisterPHModificator    ( const PHY_Posture& postureSource, const PHY_Posture& postureTarget, double rModificatorValue );
    //@}
    //! @name Helpers
    //@{
    struct LoadingWrapper;
    static void ReadLauncher( xml::xistream& xis );
    void ReadDirect         ( xml::xistream& xis );
    void ReadModifier       ( xml::xistream& xis, const PHY_Posture& postureSource );
    //@}

private:
    const std::string       strName_;

    bool                    bDirectFire_;
    T_PhModificatorsVector  phModificators_;

    bool                    bIndirectFire_;

private:
    static T_LauncherTypeMap  launcherTypes_;
};

#endif // __PHY_LauncherType_h_
