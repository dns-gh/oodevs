// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef __InitialStateCrew_h_
#define __InitialStateCrew_h_

#include "ENT/ENT_Enums_Gen.h"

namespace xml
{
    class xistream;
    class xostream;
}

// =============================================================================
/** @class  InitialStateCrew
    @brief  InitialStateCrew
*/
// Created: ABR 2011-03-07
// =============================================================================
class InitialStateCrew
{
public:
    //! @name Constructors/Destructor
    //@{
             InitialStateCrew( E_HumanRank rank, E_HumanState state, E_InjuriesSeriousness injurySeriousness, bool psyop, bool contaminated, unsigned int number );
    explicit InitialStateCrew( xml::xistream& xis );
    virtual ~InitialStateCrew();
    //@}

    //! @name Operations
    //@{
    void Serialize( xml::xostream& xos ) const;
    bool operator==( const InitialStateCrew& object ) const;
    bool operator!=( const InitialStateCrew& object ) const;
    //@}

private:
    //! @name Helpers
    //@{
    void LoadInjuries( const std::string& injuries );
    const std::string SaveInjuries() const;
    //@}

public:
    //! @name Types
    //@{
    typedef std::map< unsigned int, E_InjuriesSeriousness >   T_Injuries;
    typedef T_Injuries::iterator                             IT_Injuries;
    typedef T_Injuries::const_iterator                      CIT_Injuries;
    //@}

public:
    //! @name Member data
    //@{
    E_HumanRank  rank_;
    E_HumanState state_;
    E_InjuriesSeriousness currentSeriousness_; // $$$$ ABR 2011-07-20: Temporaire en attente de l'histoire 660
    T_Injuries   injuries_; // $$$$ ABR 2011-07-20: En avance sur l'histoire 660
    bool         psyop_;
    bool         contaminated_;
    unsigned int number_;
    //@}
};

#endif // __InitialStateCrew_h_
