// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/entities/agents/perceptions/PHY_PerceptionLevel.h $
// $Author: Jvt $
// $Modtime: 16/03/05 19:05 $
// $Revision: 2 $
// $Workfile: PHY_PerceptionLevel.h $
//
// *****************************************************************************

#ifndef __PHY_PerceptionLevel_h_
#define __PHY_PerceptionLevel_h_

#include "game_asn/Simulation.h"

// =============================================================================
// @class  PHY_PerceptionLevel
// Created: JVT 2004-08-03
// =============================================================================
class PHY_PerceptionLevel
{
    MT_COPYNOTALLOWED( PHY_PerceptionLevel )

public:
    static const PHY_PerceptionLevel identified_;
    static const PHY_PerceptionLevel recognized_;
    static const PHY_PerceptionLevel detected_;
    static const PHY_PerceptionLevel notSeen_;

public:
    //! @name Manager
    //@{
    static void Initialize();
    static void Terminate ();
    //@}

    //! @name Accessors
    //@{
    const std::string& GetName() const;
    uint GetID() const;
    bool IsBestLevel() const;
    //@}

    //! @name Operators
    //@{
    void Serialize( ASN1T_EnumUnitIdentificationLevel& level ) const;
    bool operator ==( const PHY_PerceptionLevel& rhs ) const;
    bool operator !=( const PHY_PerceptionLevel& rhs ) const;
    bool operator < ( const PHY_PerceptionLevel& rhs ) const;
    bool operator <=( const PHY_PerceptionLevel& rhs ) const;
    bool operator > ( const PHY_PerceptionLevel& rhs ) const;
    bool operator >=( const PHY_PerceptionLevel& rhs ) const;
    //@}

public:
    static const PHY_PerceptionLevel& FindPerceptionLevel( uint nID );

private:
    //! @name Types
    //@{
    enum E_Level
    {
        eNotSeen    = 0,
        eDetected   = 1,
        eRecognized = 2,
        eIdentified = 3, 
    };
    //@}

private:
     PHY_PerceptionLevel( const std::string& strName, E_Level nLevel, const ASN1T_EnumUnitIdentificationLevel& nAsnID );
    ~PHY_PerceptionLevel();

private:
    //! @name types
    //@{
    typedef std::map< uint, const PHY_PerceptionLevel* > T_LevelMap;
    typedef T_LevelMap::const_iterator                   CIT_LevelMap;
    //@}

private:
    const std::string                       strName_;
    const E_Level                           nLevel_;
    const ASN1T_EnumUnitIdentificationLevel nAsnID_;
    
private:
    static T_LevelMap levels_;
};

#endif // __PHY_PerceptionLevel_h_
