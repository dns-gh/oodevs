// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 MASA Group
//
// *****************************************************************************

#ifndef __PHY_HumanRank_h_
#define __PHY_HumanRank_h_

#include "MT_Tools/MT_String.h"
#include <map>

namespace sword
{
    enum EnumHumanRank;
}

// =============================================================================
// @class  PHY_HumanRank
// Created: JVT 2004-08-03
// =============================================================================
class PHY_HumanRank : private boost::noncopyable
{
public:
    //! @name Types
    //@{
    typedef std::map< std::string, const PHY_HumanRank* > T_HumanRankMap;
    typedef T_HumanRankMap::const_iterator              CIT_HumanRankMap;
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
    static const PHY_HumanRank*  Find( unsigned int nID );
    static const PHY_HumanRank*  Find( sword::EnumHumanRank nAsnID );
    //@}

    //! @name Accessors
    //@{
    const std::string&   GetName    () const;
    unsigned int         GetID      () const;
    sword::EnumHumanRank GetAsnID   () const;
    bool                 IsCommander() const;
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
    //! @name Constructors/Destructor
    //@{
     PHY_HumanRank( const std::string& strName, E_Rank nRank, sword::EnumHumanRank nAsnID, bool bIsCommander );
    ~PHY_HumanRank();
    //@}

private:
    //! @name Member data
    //@{
    const std::string strName_;
    const E_Rank nRank_;
    const sword::EnumHumanRank nAsnID_;
    const bool bIsCommander_;
    //@}

private:
    static T_HumanRankMap humanRanks_;
};

#endif // __PHY_HumanRank_h_
