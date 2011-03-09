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
    enum E_CrewRanks  { eOfficer = 0, eWarrant = 1, ePrivate = 2 };
    enum E_CrewStates { eHealthy = 0, eWounded_ue = 1, eWounded_u1 = 2, eWounded_u2 = 3, eWounded_u3 = 4, eDead = 5 };

    //! @name Constructors/Destructor
    //@{
             InitialStateCrew( E_CrewRanks rank, E_CrewStates state, bool psyop, bool contaminated, unsigned int number );
    explicit InitialStateCrew( xml::xistream& xis );
    virtual ~InitialStateCrew();
    //@}

    //! @name Operations
    //@{
    void Serialize( xml::xostream& xos ) const;
    bool operator==( const InitialStateCrew& object ) const;
    bool operator!=( const InitialStateCrew& object ) const;
    //@}

public:
    //! @name Member data
    //@{
    E_CrewRanks  rank_;
    E_CrewStates state_;
    bool         psyop_;
    bool         contaminated_;
    unsigned int number_;
    //@}
};

#endif // __InitialStateCrew_h_
