// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#ifndef __Exercise_h_
#define __Exercise_h_

#include <boost/noncopyable.hpp>
#include "tools/ExerciseSettings.h"

namespace kernel
{
    class Controller;
}

namespace xml
{
    class xistream;
    class xostream;
}

namespace tools
{
    class ExerciseConfig;
    class SchemaWriter_ABC;
}

class ExerciseVisitor_ABC
{
public:
             ExerciseVisitor_ABC() {}
    virtual ~ExerciseVisitor_ABC() {}
    virtual void VisitBriefing( const QString& lang, const QString& briefing ) = 0;
    virtual void VisitResource( const QString& name, const tools::Path& file ) = 0;
    virtual void VisitOrderFile( const tools::Path& file ) = 0;
};

// =============================================================================
/** @class  Exercise
    @brief  Exercise meta data
*/
// Created: SBO 2010-03-08
// =============================================================================
class Exercise : private boost::noncopyable
{
public:
    //! @name Constructors/Destructor
    //@{
    explicit Exercise( kernel::Controller& controller );
    virtual ~Exercise();
    //@}

    //! @name Operations
    //@{
    void Load( xml::xistream& xis );
    void Purge();
    void Serialize( const tools::ExerciseConfig& config, const tools::SchemaWriter_ABC& schemaWriter ) const;
    void Accept( ExerciseVisitor_ABC& visitor ) const;
    //@}

    //! @name Accessors
    //@{
    tools::ExerciseSettings& GetSettings();
    QString GetName() const;
    void SetName( const QString& name );
    void SetBriefing( const QString& lang, const QString& text );
    void AddResource( const QString& name, const tools::Path& file );
    void AddOrderFile( const tools::Path& file );
    void SetActionPlanning( const tools::Path& filename );
    void SetExerciseValidity( bool isValid );
    void ClearResources();
    void ClearOrderFiles();
    //@}

private:
    //! @name Helpers
    //@{
    void ReadBriefing( xml::xistream& xis );
    void ReadResource( xml::xistream& xis );
    void ReadOrderFile( xml::xistream& xis );
    void SerializeBriefings( xml::xostream& xos ) const;
    void SerializeResources( xml::xostream& xos ) const;
    void SerializeOrderFiles( xml::xostream& xos ) const;
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controller& controller_;
    tools::ExerciseSettings settings_;
    QString name_;
    std::map< QString, QString > briefings_;
    std::map< QString, tools::Path > resources_;
    tools::Path::T_Paths orderFiles_;
    tools::Path actionPlanning_;
    bool isValid_;
    //@}
};

#endif // __Exercise_h_
