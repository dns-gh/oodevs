// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#ifndef __SuccessFactorActionTypes_h_
#define __SuccessFactorActionTypes_h_

#include <tools/Resolver.h>

namespace tools
{
    class ExerciseConfig;
}

namespace xml
{
    class xistream;
}

class SuccessFactorActionType;

// =============================================================================
/** @class  SuccessFactorActionTypes
    @brief  SuccessFactorActionTypes
*/
// Created: SBO 2009-06-16
// =============================================================================
class SuccessFactorActionTypes : public tools::Resolver< SuccessFactorActionType, QString >
{
public:
    //! @name Constructors/Destructor
    //@{
             SuccessFactorActionTypes();
    virtual ~SuccessFactorActionTypes();
    //@}

    //! @name Operations
    //@{
    void Load( const tools::ExerciseConfig& config, const tools::Path& file );
    void Purge();
    //@}

private:
    //! @name Copy/Assignment
    //@{
    SuccessFactorActionTypes( const SuccessFactorActionTypes& );            //!< Copy constructor
    SuccessFactorActionTypes& operator=( const SuccessFactorActionTypes& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    void ReadAction( xml::xistream& xis );
    void Read( xml::xistream& xis );
    //@}
};

#endif // __SuccessFactorActionTypes_h_
