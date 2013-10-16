// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef aar_PowerIndicators_h
#define aar_PowerIndicators_h

#include <boost/noncopyable.hpp>
#include <tools/Resolver.h>

namespace tools
{
    class ExerciseConfig;
}

namespace xml
{
    class xistream;
}

namespace aar
{

class PowerIndicator;

// =============================================================================
/** @class  PowerIndicators
    @brief  Resolver on PowerIndicator
*/
// Created: ABR 2011-02-09
// =============================================================================
class PowerIndicators : public tools::Resolver< PowerIndicator >
                      , public tools::StringResolver< PowerIndicator >
                      , private boost::noncopyable
{
public:
    //! @name Constructors/Destructor
    //@{
             PowerIndicators();
    explicit PowerIndicators( const tools::ExerciseConfig& config );
    virtual ~PowerIndicators();
    //@}

    //! @name Operations
    //@{
    void Load( const tools::ExerciseConfig& config );
    void Purge();
    //@}

private:
    //! @name Helpers
    //@{
    void ReadEquipment( xml::xistream& xis );
    void ReadPowerIndicator( xml::xistream& xis );
    //@}
};

} // namespace aar

#endif // aar_PowerIndicators_h
