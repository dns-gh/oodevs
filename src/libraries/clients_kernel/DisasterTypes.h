// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#ifndef kernel_DisasterTypes_h
#define kernel_DisasterTypes_h

#include <tools/Resolver.h>

namespace tools
{
    class ExerciseConfig;
}

namespace kernel
{
    class DisasterType;
// =============================================================================
/** @class  DisasterTypes
    @brief  Disaster types
*/
// Created: LGY 2012-11-19
// =============================================================================
class DisasterTypes : public tools::Resolver< DisasterType, std::string >
{
public:
    //! @name Constructors/Destructor
    //@{
             DisasterTypes();
    virtual ~DisasterTypes();
    //@}

    //! @name Operations
    //@{
    void Load( const tools::ExerciseConfig& config );
    void Purge();
    //@}

private:
    //! @name Helpers
    //@{
    void ReadDisasters( xml::xistream& xis );
    void ReadDisaster( xml::xistream& xis );
    //@}
};

}

#endif // kernel_DisasterTypes_h
