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
class MIL_Army;

// =============================================================================
// @class  MIL_KnowledgeGroupType
// Created: JVT 2004-08-03
// =============================================================================
class MIL_KnowledgeGroupType : private boost::noncopyable
{

public:
    MIL_KnowledgeGroupType( const std::string& strName, xml::xistream& xis );
    ~MIL_KnowledgeGroupType();

    //! @name Manager
    //@{
    static void Initialize( xml::xistream& xis );
    static void Terminate ();

    static const MIL_KnowledgeGroupType* FindType( const std::string& strName );
    static const MIL_KnowledgeGroupType* FindType( uint nID );
    //@}

    //! @name Instanciation
    //@{
    MIL_KnowledgeGroup& InstanciateKnowledgeGroup( uint nID, MIL_Army& army ) const;
    //@}

    //! @name Accessors
    //@{
          MT_Float     GetKnowledgeAgentMaxLifeTime                   () const;
          MT_Float     GetKnowledgeAgentMaxDistBtwKnowledgeAndRealUnit() const;
          MT_Float     GetKnowledgeAgentExtrapolationTime             () const;
          MT_Float     GetKnowledgePopulationMaxLifeTime              () const;
          MT_Float     GetKnowledgeCommunicationDelay                 () const;
    const std::string& GetName                                        () const;
          uint         GetID                                          () const;
    //@}

private:
    //! @name Types
    //@{
    typedef std::map< std::string, const MIL_KnowledgeGroupType*, sCaseInsensitiveLess > T_KnowledgeGroupTypeMap;
    typedef T_KnowledgeGroupTypeMap::const_iterator                                      CIT_KnowledgeGroupTypeMap;
    //@}

    //! @name Helper
    //@{
    struct LoadingWrapper;
    static void ReadKnowledgeGroup( xml::xistream& xis );
    //@}

private:
    const std::string strName_;
    const uint        nID_;
          MT_Float    rKnowledgeAgentMaxLifeTime_;
          MT_Float    rKnowledgeAgentMaxDistBtwKnowledgeAndRealUnit_;
          MT_Float    rKnowledgeAgentExtrapolationTime_;

          MT_Float    rKnowledgePopulationMaxLifeTime_;
          MT_Float    rCommunicationDelay_;

private:
    static T_KnowledgeGroupTypeMap knowledgeGroupTypes_;
    static uint                    nNextID_;
};

#endif // __MIL_KnowledgeGroupType_h_
