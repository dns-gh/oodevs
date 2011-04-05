// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef __Criteria_h_
#define __Criteria_h_

#include <boost/noncopyable.hpp>

namespace mission_tester
{
// =============================================================================
/** @class  Criteria
    @brief  Criteria
*/
// Created: PHC 2011-04-04
// =============================================================================
class Criteria : private boost::noncopyable
{

public:
    //! @name Constructors/Destructor
    //@{
             Criteria();
    explicit Criteria( const std::string& value );
    virtual ~Criteria();
    //@}

    //! @name Operations
    //@{
    void AddCriterion( const std::string& value );
    bool Matches( const std::string& value ) const;
    //@}

private:
    //! @name types
    //@{
    typedef std::vector< std::string >   T_Criteria;
    typedef T_Criteria::const_iterator CIT_Criteria;
    //@}

    //! @name Member data
    //@{
    T_Criteria criteria_;
    //@}
};
}

#endif // __Criteria_h_
