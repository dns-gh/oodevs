// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#ifndef __SuccessFactorsModel_h_
#define __SuccessFactorsModel_h_

#include "tools/Resolver.h"

namespace tools
{
    class ExerciseConfig;
}

namespace xml
{
    class xistream;
    class xostream;
}

class ModelChecker_ABC;
class SuccessFactor;
class SuccessFactorFactory_ABC;

// =============================================================================
/** @class  SuccessFactorsModel
    @brief  SuccessFactorsModel
*/
// Created: SBO 2009-06-15
// =============================================================================
class SuccessFactorsModel : public tools::Resolver< SuccessFactor, QString >
{
public:
    //! @name Constructors/Destructor
    //@{
    explicit SuccessFactorsModel( SuccessFactorFactory_ABC& factory );
    virtual ~SuccessFactorsModel();
    //@}

    //! @name Operations
    //@{
    void Create( const QString& name );
    void Delete( const SuccessFactor& factor );
    void Purge();
    void Load( const std::string& file );
    bool CheckValidity( ModelChecker_ABC& checker ) const;
    void Serialize( const std::string& file ) const;
    void SerializeScript( const tools::ExerciseConfig& config ) const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    SuccessFactorsModel( const SuccessFactorsModel& );            //!< Copy constructor
    SuccessFactorsModel& operator=( const SuccessFactorsModel& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    void Serialize( xml::xostream& xos ) const;
    void ReadFactor( xml::xistream& xis );
    //@}

private:
    //! @name Member data
    //@{
    SuccessFactorFactory_ABC& factory_;
    //@}
};

#endif // __SuccessFactorsModel_h_
