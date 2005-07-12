//*****************************************************************************
//
// $Created: FBD 03-07-31 $
// $Archive: /MVW_v10/Build/SDK/MOS_Light/src/MOS_TestManager.h $
// $Author: Nld $
// $Modtime: 10/09/04 11:40 $
// $Revision: 5 $
// $Workfile: MOS_TestManager.h $
//
//*****************************************************************************

#ifndef __MOS_TestManager_h_
#define __MOS_TestManager_h_

#include "MOS_Types.h"

#include "MT/MT_Archive/MT_FlatBinaryOutputArchive.h"
#include "MT/MT_Archive/MT_FlatBinaryInputArchive.h"

class MOS_AgentTest;
class MOS_Agent;
class MOS_Mission_ABC;

class TmpOutputArchive;


//*****************************************************************************
// Created: FBD 03-07-31
//*****************************************************************************
class MOS_TestManager
{
    MT_COPYNOTALLOWED( MOS_TestManager )

public:
    //-------------------------------------------------------------------------
    /** @name Types */
    //-------------------------------------------------------------------------
    //@{
    typedef std::set< MOS_AgentTest* >          T_AgentTestPtrSet;
    typedef T_AgentTestPtrSet::iterator         IT_AgentTestPtrSet;
    typedef T_AgentTestPtrSet::const_iterator   CIT_AgentTestPtrSet;
    //}@

public:
    //-------------------------------------------------------------------------
    /** @name Constructor/Destructor */
    //-------------------------------------------------------------------------
    //@{
    explicit MOS_TestManager();
    virtual ~MOS_TestManager();
    //}@

    //-------------------------------------------------------------------------
    /** @name Manager */
    //-------------------------------------------------------------------------
    //@{
    void            Initialize();
    void            Terminate();
    MOS_AgentTest&  CreateAgentTest     ( MOS_Agent&        agent );
    void            DeleteAgentTest     ( MOS_Agent&        agent );
    void            DeleteAgentTest     ( MOS_AgentTest&    agent );
    void            RegisterAgentTest   ( MOS_AgentTest&    agent );
    void            UnregisterAgentTest ( MOS_AgentTest&    agent );
    //}@


    //-------------------------------------------------------------------------
    /** @name Timer */
    //-------------------------------------------------------------------------
    //@{
    void            Update      ( int nTime );
    void            RecordRandom( int nTime );
    void            RecordManual( int nTime, MOS_Agent& agent, MOS_Mission_ABC& mission );
    void            Replay      ( int nTime );
    
    void            InitReplay();
    void            InitRecord();
    void            SayMissionFinished( MOS_Agent& agent, uint nCurTime );
    //}@

    //-------------------------------------------------------------------------
    /** @State */
    //-------------------------------------------------------------------------
    //@{
    bool                        GetFlagTest        () const;
    void                        SetFlagTest        ( bool bTest );
    void                        SetMinimumTimeTest  ( int nTime );

    bool                        GetFlagReplay      () const;
    void                        SetFlagReplay      ( bool bReplay );
    void                        SetFlagRec         ( bool bRec );
    bool                        GetFlagRec         () const;
    MT_FlatBinaryOutputArchive& GetRecArchive();
    std::string                 GetFileNameOut() const;
    void                        SetFileNameOut( std::string& sName );
    //}@

private:
    T_AgentTestPtrSet   agentTestPtrSet_;

    MIL_AgentID         nNextId_; 
    int                 nCurTime_;
    int                 nMinimumTimeTest_;
    int                 nTimeNextOrder_;
    int                 nState_;
    int                 nOldFrame_;


    bool            bTest_;
    bool            bReplay_;
    bool            bRec_;
    std::string     sFileNameOut_;
    
    static TmpOutputArchive             archiveOutput_;
    static MT_FlatBinaryInputArchive    archiveInput_;

};

#ifdef MOS_USE_INLINE
#	include "MOS_TestManager.inl"
#endif


#endif // __MOS_TestManager_h_