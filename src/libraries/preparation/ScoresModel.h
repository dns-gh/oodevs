// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#ifndef __ScoresModel_h_
#define __ScoresModel_h_

#include "clients_kernel/Resolver.h"

namespace xml
{
    class xistream;
    class xostream;
}

class ModelChecker_ABC;
class Score_ABC;
class ScoreFactory_ABC;

// =============================================================================
/** @class  ScoresModel
    @brief  ScoresModel
*/
// Created: SBO 2009-04-16
// =============================================================================
class ScoresModel : public kernel::Resolver< Score_ABC, QString >
{

public:
    //! @name Constructors/Destructor
    //@{
    explicit ScoresModel( const ScoreFactory_ABC& factory );
    virtual ~ScoresModel();
    //@}

    //! @name Operations
    //@{
    void Create( const QString& name );
    void Delete( const Score_ABC& score );
    void Purge();
    void Load( const std::string& file );
    bool CheckValidity( ModelChecker_ABC& checker ) const;
    void Serialize( const std::string& file ) const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    ScoresModel( const ScoresModel& );            //!< Copy constructor
    ScoresModel& operator=( const ScoresModel& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    void Serialize( xml::xostream& xos ) const;
    void ReadScore( xml::xistream& xis );
    //@}

private:
    //! @name Member data
    //@{
    const ScoreFactory_ABC& factory_;
    //@}
};

#endif // __ScoresModel_h_
