// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2014 MASA Group
//
// *****************************************************************************

#ifndef __DevFeatures_h_
#define __DevFeatures_h_

#include <boost/noncopyable.hpp>
#include <unordered_set>

namespace tools
{

// Returns the set of features extracted from input string. Identifiers are
// split on ';' and trimmed.
std::unordered_set< std::string > SplitFeatures( const std::string& s );
std::string JoinFeatures( const std::unordered_set< std::string >& features );
const std::vector< std::string >& GetAvailableFeatures();

// =============================================================================
/** @class  DevFeatures
    @brief  DevFeatures
*/
// Created: LGY 2014-08-05
// =============================================================================
class DevFeatures : private boost::noncopyable
{
public:
    static DevFeatures& Instance();

    void Initialize( const std::string& features );
    bool HasFeature( const std::string& feature ) const;

private:
             DevFeatures();
    virtual ~DevFeatures();

private:
    std::unordered_set< std::string > features_;
};

}
#endif // __DevFeatures_h_
