// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Agents/Units/Humans/MIL_PopulationAttitude.h $
// $Author: Jvt $
// $Modtime: 14/04/05 17:05 $
// $Revision: 3 $
// $Workfile: MIL_PopulationAttitude.h $
//
// *****************************************************************************

#ifndef __MIL_PopulationAttitude_h_
#define __MIL_PopulationAttitude_h_

#include "MIL.h"

#include "Network/NET_ASN_Types.h"

// =============================================================================
// @class  MIL_PopulationAttitude
// Created: JVT 2004-08-03
// =============================================================================
class MIL_PopulationAttitude
{
    MT_COPYNOTALLOWED( MIL_PopulationAttitude )

public:
    //! @name Manager
    //@{
    static void Initialize();
    static void Terminate ();

    static const MIL_PopulationAttitude* Find( const std::string& strName );
    static const MIL_PopulationAttitude* Find( ASN1T_EnumPopulationAttitude nAsnID );
    //@}

    //! @name Accessors
    //@{
    const std::string&           GetName () const;
    uint                         GetID   () const;
    ASN1T_EnumPopulationAttitude GetAsnID() const;
    //@}

    //! @name Operators
    //@{
    bool operator==( const MIL_PopulationAttitude& rhs ) const;
    bool operator!=( const MIL_PopulationAttitude& rhs ) const;
    //@}

private:
    //! @name Types
    //@{
    enum E_Attitude
    {
        eCalme     = 0,
        eAgitee    = 1,
        eExcitee   = 2,
        eAgressive = 3
    };

    typedef std::map< std::string, const MIL_PopulationAttitude*, sCaseInsensitiveLess > T_AttitudeMap;
    typedef T_AttitudeMap::const_iterator                                                CIT_AttitudeMap;
    //@}

private:
     MIL_PopulationAttitude( const std::string& strName, E_Attitude nAttitude, ASN1T_EnumPopulationAttitude nAsnID );
    ~MIL_PopulationAttitude();

private:
    const std::string                  strName_;
    const E_Attitude                   nAttitude_;
    const ASN1T_EnumPopulationAttitude nAsnID_;

private:
    static T_AttitudeMap attitudes_;
};

#include "MIL_PopulationAttitude.inl"

#endif // __MIL_PopulationAttitude_h_
