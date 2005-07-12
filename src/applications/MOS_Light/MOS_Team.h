// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: NLD 2004-03-18 $
// $Archive: /MVW_v10/Build/SDK/MOS_Light/src/MOS_Team.h $
// $Author: Nld $
// $Modtime: 14/02/05 15:56 $
// $Revision: 2 $
// $Workfile: MOS_Team.h $
//
// *****************************************************************************

#ifndef __MOS_Team_h_
#define __MOS_Team_h_

#include "MOS_Types.h"
#include "MOS_ASN_Types.h"

class MOS_ObjectKnowledge;
class MOS_ObjectKnowledge_ListView;

// =============================================================================

// Created: NLD 2004-03-18
// =============================================================================
class MOS_Team
{
    MT_COPYNOTALLOWED( MOS_Team );

public:
    //! @name Types
    //@{
    typedef std::map< uint, MOS_ObjectKnowledge* >   T_ObjectKnowledgeMap;
    typedef T_ObjectKnowledgeMap::iterator           IT_ObjectKnowledgeMap;
    typedef T_ObjectKnowledgeMap::const_iterator     CIT_ObjectKnowledgeMap;
    //@}

public:
    //! @name Constructors/Destructor
    //@{
     MOS_Team( uint nID, DIN::DIN_Input& input );
    ~MOS_Team();
    //@}

    //! @name Accessors
    //@{
    uint                        GetID  () const;
    const std::string&          GetName() const;
    const T_ObjectKnowledgeMap& GetObjectKnowledges() const;
    const GFX_Color&            GetColor() const;
    //@}

    //! @name Network events
    //@{
    void OnReceiveMsgObjectKnowledgeCreation   ( const ASN1T_MsgObjectKnowledgeCreation&    asnMsg );
    void OnReceiveMsgObjectKnowledgeUpdate     ( const ASN1T_MsgObjectKnowledgeUpdate&      asnMsg );
    void OnReceiveMsgObjectKnowledgeDestruction( const ASN1T_MsgObjectKnowledgeDestruction& asnMsg );
    //@}

    //! @name Knowledges
    //@{
    void SetObjectKnowledgeListView( MOS_ObjectKnowledge_ListView* pListView );
    void DestroyAllKnowledges();
    //@}

    //! @name Display
    //@{
    bool ShouldDrawKnowledges() const;
    void NotifyShouldDrawKnowledges( bool bDraw );
    void Draw();
    //@}


private:
    std::string         strName_;
    uint32              nID_;
    T_ObjectKnowledgeMap objectKnowledges_;
    GFX_Color           color_;

    MOS_ObjectKnowledge_ListView* pKnowledgeListView_;

    bool bShouldDrawKnowledges_;

    uint nIdx_;
};

#include "MOS_Team.inl"

#endif // __MOS_Team_h_
