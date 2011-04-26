// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef __PowerModificator_h_
#define __PowerModificator_h_

#include <boost/noncopyable.hpp>

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

class PowerModificator;

// =============================================================================
/** @class  PowerModificator
    @brief  PowerModificator
*/
// Created: FPO 2011-04-04
// =============================================================================
class PowerModificator : private boost::noncopyable
{

public:
    //! @name Constructors/Destructor
    //@{
             PowerModificator();
    explicit PowerModificator( const tools::ExerciseConfig& config );
    virtual ~PowerModificator();
    //@}

    //! @name Operations
    //@{
    void Load( const tools::ExerciseConfig& config );
    //@}

    //! @name
    //@{
    double GetAvailableModificator() const;
    double GetRepairableModificator() const;
    double GetRepairingModificator() const;
    double GetCapturedModificator() const;
    double GetUnavailableModificator() const;
    //@}

private:
    //! @name Helpers
    //@{
    void ReadPowerModificator( xml::xistream& xis );
    //@}

private:
    //! @name Member data
    //@{
    double available_;
    double repairable_;
    double repairing_;
    double captured_;
    double unavailable_;
    //@}
};

} // Namespace aar

#endif // __PowerModificator_h_
