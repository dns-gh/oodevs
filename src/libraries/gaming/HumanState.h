// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef __HumanState_h_
#define __HumanState_h_

#include <boost/noncopyable.hpp>
#include "ENT/ENT_Enums_Gen.h"

namespace sword
{
    class HumanDotations_HumanDotation;
}

// =============================================================================
/** @class  HumanState
    @brief  HumanState
*/
// Created: ABR 2011-07-25
// =============================================================================
class HumanState
{
public:
    //! @name Constructors/Destructor
    //@{
             HumanState();
             HumanState( const HumanState& );
    explicit HumanState( const sword::HumanDotations_HumanDotation& );
    virtual ~HumanState();
    //@}

    //! @name Operations
    //@{
    void Update( const sword::HumanDotations_HumanDotation& );

    bool operator==( const HumanState& other ) const;
    //@}

public:
    //! @name Types
    //@{
    typedef std::pair< unsigned int, E_InjuriesSeriousness >  T_Injury;

    typedef std::vector< T_Injury >                           T_Injuries;
    typedef T_Injuries::iterator                             IT_Injuries;
    typedef T_Injuries::const_iterator                      CIT_Injuries;
    //@}

public:
    //! @name Member data
    //@{
    E_HumanRank     rank_;
    E_HumanState    state_;
    E_HumanLocation location_;
    T_Injuries      injuries_;
    bool            psyop_;
    bool            contaminated_;
    int             quantity_;
    //@}
};

#endif // __HumanState_h_
