// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef __HumanStateHelper_h_
#define __HumanStateHelper_h_

#include <boost/noncopyable.hpp>

class PHY_HumanRank;
class PHY_HumanWound;
class Human_ABC;

// =============================================================================
/** @class  HumanState
    @brief  HumanState
*/
// Created: ABR 2011-03-08
// =============================================================================
class HumanState : private boost::noncopyable
{
public:
    //! @name Constructors/Destructor
    //@{
             HumanState( const PHY_HumanRank& rank, const PHY_HumanWound& state, bool contaminated, bool psyop, unsigned int number );
    virtual ~HumanState();
    //@}

    //! @name Member data
    //@{
    const PHY_HumanRank&  rank_;
    const PHY_HumanWound& state_;
    bool                  contaminated_;
    bool                  psyop_;
    unsigned int          number_;
    //@}
};

// =============================================================================
/** @class  HumanStateHelper
    @brief  HumanStateHelper
*/
// Created: ABR 2011-03-08
// =============================================================================
class HumanStateHelper : private boost::noncopyable
{
public:
    //! @name Types
    //@{
    typedef std::vector< HumanState* >            T_HumansStateVector;
    typedef T_HumansStateVector::iterator        IT_HumansStateVector;
    typedef T_HumansStateVector::const_iterator CIT_HumansStateVector;
    //@}

public:
    //! @name Constructors/Destructor
    //@{
             HumanStateHelper();
    virtual ~HumanStateHelper();
    //@}

    //! @name Operations
    //@{
    const T_HumansStateVector& GetHumans() const;
    void AddHumanMergeIfNeeded( const Human_ABC& human );
    //@}

private:
    //! @name Member data
    //@{
    T_HumansStateVector humans_;
    //@}
};

#endif // __HumanStateHelper_h_
