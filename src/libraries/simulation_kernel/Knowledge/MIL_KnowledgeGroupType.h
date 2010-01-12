// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/knowledge/MIL_KnowledgeGroupType.h $
// $Author: Nld $
// $Modtime: 21/04/05 19:07 $
// $Revision: 5 $
// $Workfile: MIL_KnowledgeGroupType.h $
//
// *****************************************************************************

#ifndef __MIL_KnowledgeGroupType_h_
#define __MIL_KnowledgeGroupType_h_

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
    ~MIL_KnowledgeGroupType();

    //! @name Manager
    //@{
    static void Initialize( xml::xistream& xis, double timeFactor );
    static void Initialize( xml::xistream& xis );
    static void Terminate ();

    static const MIL_KnowledgeGroupType* FindType( const std::string& strName );
    static const MIL_KnowledgeGroupType* FindType( uint nID );
    //@}

    //! @name Instanciation
    //@{
    MIL_KnowledgeGroup& InstanciateKnowledgeGroup( uint nID, MIL_Army_ABC& army ) const;
    //@}

    //! @name Accessors
    //@{
          double       GetKnowledgeAgentMaxLifeTime                   () const;
          double       GetKnowledgeAgentMaxDistBtwKnowledgeAndRealUnit() const;
          double       GetKnowledgeAgentExtrapolationTime             () const;
          double       GetKnowledgePopulationMaxLifeTime              () const;
          double       GetKnowledgeCommunicationDelay                 () const;
    const std::string& GetName                                        () const;
          uint         GetID                                          () const;
    //@}

private:
   
    MIL_KnowledgeGroupType( const std::string& strName, xml::xistream& xis, double   timeFactor );

    //! @name Types
    //@{
    typedef std::map< std::string, const MIL_KnowledgeGroupType*, sCaseInsensitiveLess > T_KnowledgeGroupTypeMap;
    typedef T_KnowledgeGroupTypeMap::const_iterator                                      CIT_KnowledgeGroupTypeMap;
    //@}

    //! @name Helper
    //@{
    struct LoadingWrapper;
    static void ReadKnowledgeGroup( xml::xistream& xis, double timeFactor );
    //@}

private:
    const std::string strName_;
    const uint        nID_;
          double      rKnowledgeAgentMaxLifeTime_;
          double      rKnowledgeAgentMaxDistBtwKnowledgeAndRealUnit_;
          double      rKnowledgeAgentExtrapolationTime_;

          double      rKnowledgePopulationMaxLifeTime_;
          double      rCommunicationDelay_;

private:
    static T_KnowledgeGroupTypeMap knowledgeGroupTypes_;
    static uint                    nNextID_;
};

#endif // __MIL_KnowledgeGroupType_h_
