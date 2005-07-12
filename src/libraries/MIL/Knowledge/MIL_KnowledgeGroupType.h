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

#include "MIL.h"

class MIL_KnowledgeGroup;
class MIL_Army;

// =============================================================================
// @class  MIL_KnowledgeGroupType
// Created: JVT 2004-08-03
// =============================================================================
class MIL_KnowledgeGroupType
{
    MT_COPYNOTALLOWED( MIL_KnowledgeGroupType )

public:
     MIL_KnowledgeGroupType( const std::string& strName, MIL_InputArchive& archive );
    ~MIL_KnowledgeGroupType();

    //! @name Manager
    //@{
    static void Initialize( MIL_InputArchive& archive );
    static void Terminate ();

    static const MIL_KnowledgeGroupType* FindType( const std::string& strName );
    static const MIL_KnowledgeGroupType* FindType( uint nID );
    //@}

    //! @name Instanciation
    //@{
    MIL_KnowledgeGroup& InstanciateKnowledgeGroup( uint nID, MIL_Army& army, MIL_InputArchive& archive ) const;
    //@}

    //! @name Accessors
    //@{
          MT_Float     GetKnowledgeMaxLifeTime                   () const;
          MT_Float     GetKnowledgeMaxDistBtwKnowledgeAndRealUnit() const;
          MT_Float     GetKnowledgeExtrapolationTime             () const;
    const std::string& GetName                                   () const;
          uint         GetID                                     () const;
    //@}

private:
    //! @name Types
    //@{
    typedef std::map< std::string, const MIL_KnowledgeGroupType*, sCaseInsensitiveLess > T_KnowledgeGroupTypeMap;
    typedef T_KnowledgeGroupTypeMap::const_iterator                                      CIT_KnowledgeGroupTypeMap;
    //@}

private:
    const std::string strName_;
    const uint        nID_;
          MT_Float    rKnowledgeMaxLifeTime_;
          MT_Float    rKnowledgeMaxDistBtwKnowledgeAndRealUnit_;
          MT_Float    rKnowledgeExtrapolationTime_;

private:
    static T_KnowledgeGroupTypeMap knowledgeGroupTypes_;
    static uint                    nNextID_;
};

#include "MIL_KnowledgeGroupType.inl"

#endif // __MIL_KnowledgeGroupType_h_
