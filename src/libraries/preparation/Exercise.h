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
    virtual void VisitResource( const QString& name, const QString& file ) = 0;
};

// =============================================================================
/** @class  Exercise
    @brief  Exercise meta data
*/
// Created: SBO 2010-03-08
// =============================================================================
class Exercise
{
public:
    //! @name Constructors/Destructor
    //@{
    explicit Exercise( kernel::Controller& controller, const tools::ExerciseConfig& config );
    virtual ~Exercise();
    //@}

    //! @name Operations
    //@{
    void Load( xml::xistream& xis );
    void Purge();
    void Serialize( const std::string& file, const tools::SchemaWriter_ABC& schemaWriter ) const;
    void Accept( ExerciseVisitor_ABC& visitor ) const;
    //@}

    //! @name Accessors
    //@{
    QString GetName() const;
    void SetName( const QString& name );
    void SetBriefing( const QString& lang, const QString& text );
    void AddResource( const QString& name, const QString& file );
    void ClearResources();
    //@}

private:
    //! @name Copy/Assignment
    //@{
    Exercise( const Exercise& );            //!< Copy constructor
    Exercise& operator=( const Exercise& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    void ReadBriefing( xml::xistream& xis );
    void ReadResource( xml::xistream& xis );
    void SerializeBriefings( xml::xostream& xos ) const;
    void SerializeResources( xml::xostream& xos ) const;
    //@}

    //! @name Types
    //@{
    typedef std::map< QString, QString > T_Resources;
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controller& controller_;
    const tools::ExerciseConfig& config_;
    QString name_;
    T_Resources briefings_;
    T_Resources resources_;
    //@}
};

#endif // __Exercise_h_
