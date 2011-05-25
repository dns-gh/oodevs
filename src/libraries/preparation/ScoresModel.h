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

#include "tools/Resolver.h"

namespace kernel
{
    class DotationType;
    class EquipmentType;
}

namespace xml
{
    class xistream;
    class xostream;
}

namespace tools
{
    class Loader_ABC;
    class SchemaWriter_ABC;
}

class ModelChecker_ABC;
class Score_ABC;
class ScoreFactory_ABC;
class TeamsModel;

// =============================================================================
/** @class  ScoresModel
    @brief  ScoresModel
*/
// Created: SBO 2009-04-16
// =============================================================================
class ScoresModel : public tools::Resolver< Score_ABC, QString >
{
public:
    //! @name Constructors/Destructor
    //@{
             ScoresModel( const ScoreFactory_ABC& factory, const TeamsModel& teams, const tools::Resolver_ABC< kernel::DotationType >& dotations, const tools::Resolver_ABC< kernel::EquipmentType >& equipments );
    virtual ~ScoresModel();
    //@}

    //! @name Operations
    //@{
    void Create( const QString& name );
    void Delete( const Score_ABC& score );
    void Purge();
    void Load( const tools::Loader_ABC& fileLoader, const std::string& file );
    bool CheckValidity( ModelChecker_ABC& checker, const tools::SchemaWriter_ABC& schemaWriter ) const;
    void Serialize( const std::string& file, const tools::SchemaWriter_ABC& schemaWriter ) const;
    void GenerateScoresFromTemplate( const tools::Loader_ABC& fileLoader );
    //@}

private:
    //! @name Copy/Assignment
    //@{
    ScoresModel( const ScoresModel& );            //!< Copy constructor
    ScoresModel& operator=( const ScoresModel& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    void ReadTemplate( xml::xistream& xis );
    void Read( xml::xistream& xis );
    void Serialize( xml::xostream& xos, const tools::SchemaWriter_ABC& schemaWriter ) const;
    void ReadScore( xml::xistream& xis );
    void ReadForeach( xml::xistream& xis );
    void IterateAmmunitions( const std::string& content, const std::string& toReplace, const std::string& name, const std::string& toReplaceParties );
    void IterateEquipments( const std::string& content, const std::string& toReplace, const std::string& name, const std::string& toReplaceParties );
    void IterateParties( const std::string& content, const std::string& toReplace, const std::string& name );
    void CreateScoreFromString( const std::string& buffer );
    //@}

private:
    //! @name Member data
    //@{
    const ScoreFactory_ABC& factory_;
    const TeamsModel& teams_;
    const tools::Resolver_ABC< kernel::DotationType >& dotations_;
    const tools::Resolver_ABC< kernel::EquipmentType >& equipments_;
    //@}
};

#endif // __ScoresModel_h_
