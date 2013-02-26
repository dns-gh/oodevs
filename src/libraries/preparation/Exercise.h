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

class ExerciseVisitor_ABC;

// =============================================================================
/** @class  Exercise
    @brief  Exercise meta data
*/
// Created: SBO 2010-03-08
// =============================================================================
class Exercise : private boost::noncopyable
{
public:
    //! @name Types
    //@{
    enum E_Sics
    {
        eSicAtlas,
        eSicDQPManager,
        eSicSICF,
        eSicSIR,
        eSicNbr
    };
    //@}

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
    void SerializeAndSign( const tools::ExerciseConfig& config, const tools::SchemaWriter_ABC& schemaWriter ) const;
    bool NeedsSaving() const;
    void Accept( ExerciseVisitor_ABC& visitor ) const;
    //@}

    //! @name Accessors
    //@{
    tools::ExerciseSettings& GetSettings();
    QString GetName() const;
    void SetName( const QString& name );
    void SetBriefing( const QString& lang, const QString& text );
    void AddResource( const QString& name, const QString& file );
    void SetActionPlanning( const std::string& filename );
    std::string GetActionPlanning() const;
    void ClearResources();
    void AcceptSic( ExerciseVisitor_ABC& visitor, E_Sics sic ) const;
    void SetSicVersion( E_Sics sic, unsigned int revision );
    //@}

private:
    //! @name Helpers
    //@{
    void ReadBriefing( xml::xistream& xis );
    void ReadResource( xml::xistream& xis );
    void SerializeBriefings( xml::xostream& xos ) const;
    void SerializeResources( xml::xostream& xos ) const;
    bool ReadAllowedSicVersions();
    void ReadSicVersions( const std::string& tag, xml::xistream& xis );
    void ReadSicVersion( xml::xistream& xis, const std::string& sic );
    void CheckSic( E_Sics sic );
    //@}

    //! @name Types
    //@{
    typedef std::map< QString, QString > T_Resources;
    struct T_AllowedSicVersion
    {
        T_AllowedSicVersion( unsigned int revision, const std::string& name )
            : revision_( revision )
            , name_( name )
        {
            // NOTHING
        }
        unsigned int revision_;
        std::string name_;
    };
    typedef std::vector< T_AllowedSicVersion > T_AllowedSicVersions;
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controller& controller_;
    tools::ExerciseSettings settings_;
    QString name_;
    T_Resources briefings_;
    T_Resources resources_;
    std::string actionPlanning_;
    T_AllowedSicVersions allowedSicVersions_[ eSicNbr ];
    std::string sicVersions_[ eSicNbr ];
    bool needSaving_;
    //@}
};

class ExerciseVisitor_ABC
{
public:
    ExerciseVisitor_ABC() {}
    virtual ~ExerciseVisitor_ABC() {}
    virtual void VisitBriefing( const QString& lang, const QString& briefing ) = 0;
    virtual void VisitResource( const QString& name, const QString& file ) = 0;
    virtual void VisitSicVersion( Exercise::E_Sics sic, unsigned int revision, const std::string& name, bool selected ) = 0;
};

#endif // __Exercise_h_
