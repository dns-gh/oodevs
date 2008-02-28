// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Agents/Units/Humans/PHY_HumanRank.h $
// $Author: Jvt $
// $Modtime: 14/04/05 17:05 $
// $Revision: 3 $
// $Workfile: PHY_HumanRank.h $
//
// *****************************************************************************

#ifndef __PHY_HumanRank_h_
#define __PHY_HumanRank_h_

#include "MIL.h"

#include "game_asn/Simulation.h"

// =============================================================================
// @class  PHY_HumanRank
// Created: JVT 2004-08-03
// =============================================================================
class PHY_HumanRank
{
    MT_COPYNOTALLOWED( PHY_HumanRank )

public:
    //! @name Types
    //@{
    typedef std::map< std::string, const PHY_HumanRank*, sCaseInsensitiveLess > T_HumanRankMap;
    typedef T_HumanRankMap::const_iterator                                      CIT_HumanRankMap;
    //@}

public:
    static const PHY_HumanRank officier_; 
    static const PHY_HumanRank sousOfficier_;
    static const PHY_HumanRank militaireDuRang_;

public:
    //! @name Manager
    //@{
    static void Initialize();
    static void Terminate ();

    static const T_HumanRankMap& GetHumanRanks();
    static const PHY_HumanRank*  Find( const std::string& strName );
    static const PHY_HumanRank*  Find( uint nID );
    static const PHY_HumanRank*  Find( ASN1T_EnumHumanRank nAsnID );
    //@}

    //! @name Accessors
    //@{
    const std::string&  GetName    () const;
    uint                GetID      () const;
    ASN1T_EnumHumanRank GetAsnID   () const;
    bool                IsCommander() const;
    //@}

    //! @name Operators
    //@{
    bool operator==( const PHY_HumanRank& rhs ) const;
    bool operator!=( const PHY_HumanRank& rhs ) const;
    //@}

private:
    //! @name Types
    //@{
    enum E_Rank
    {
        eOfficier        = 0,
        eSousOfficier    = 1,
        eMilitaireDuRang = 2
    };
    //@}

private:
     PHY_HumanRank( const std::string& strName, E_Rank nRank, ASN1T_EnumHumanRank nAsnID, bool bIsCommander );
    ~PHY_HumanRank();

private:
    const std::string         strName_;
    const E_Rank              nRank_;
    const ASN1T_EnumHumanRank nAsnID_;
    const bool                bIsCommander_;

private:
    static T_HumanRankMap humanRanks_;
};

#include "PHY_HumanRank.inl"

#endif // __PHY_HumanRank_h_
