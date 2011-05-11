// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __MIL_KnowledgeGroupType_h_
#define __MIL_KnowledgeGroupType_h_

#include "MT_Tools/MT_String.h"
#include <map>

namespace xml
{
    class xistream;
}

class MIL_KnowledgeGroup;
class MIL_Army_ABC;

// =============================================================================
// @class  MIL_KnowledgeGroupType
// Created: JVT 2004-08-03
// =============================================================================
class MIL_KnowledgeGroupType : private boost::noncopyable
{
public:
    //! @name Destructor
    //@{
    virtual ~MIL_KnowledgeGroupType();
    //@}

    //! @name Manager
    //@{
    static void InitializeWithTime( xml::xistream& xis, double timeFactor );
    static void Initialize( xml::xistream& xis );
    static void Terminate ();
    static const MIL_KnowledgeGroupType* FindType( const std::string& strName );
    static const MIL_KnowledgeGroupType* FindType( unsigned int nID );
    //@}

    //! @name Instantiation
    //@{
    MIL_KnowledgeGroup& InstanciateKnowledgeGroup( unsigned int nID, MIL_Army_ABC& army ) const;
    //@}

    //! @name Accessors
    //@{
    double GetKnowledgeAgentMaxLifeTime() const;
    double GetKnowledgeAgentMaxDistBtwKnowledgeAndRealUnit() const;
    double GetKnowledgeAgentExtrapolationTime() const;
    double GetKnowledgePopulationMaxLifeTime() const;
    double GetKnowledgeCommunicationDelay() const;
    const std::string& GetName() const;
    unsigned int GetID() const;
    //@}

private:
    MIL_KnowledgeGroupType( const std::string& strName, xml::xistream& xis, double timeFactor );

    //! @name Types
    //@{
    typedef std::map< std::string, const MIL_KnowledgeGroupType* > T_KnowledgeGroupTypeMap;
    typedef T_KnowledgeGroupTypeMap::const_iterator CIT_KnowledgeGroupTypeMap;
    //@}

    //! @name Helper
    //@{
    struct LoadingWrapper;
    static void ReadKnowledgeGroup( xml::xistream& xis, double timeFactor );
    //@}

private:
    //! @name Member data
    //@{
    const std::string  strName_;
    const unsigned int nID_;
    double rKnowledgeAgentMaxLifeTime_;
    double rKnowledgeAgentMaxDistBtwKnowledgeAndRealUnit_;
    double rKnowledgeAgentExtrapolationTime_;
    double rKnowledgePopulationMaxLifeTime_;
    double rCommunicationDelay_;
    //@}

private:
    static T_KnowledgeGroupTypeMap knowledgeGroupTypes_;
    static unsigned int nNextID_;
};

#endif // __MIL_KnowledgeGroupType_h_
