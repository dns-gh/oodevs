// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: APE 2004-10-20 $
// $Archive: /MVW_v10/Build/SDK/Light2/src/MsgRecorder.h $
// $Author: Ape $
// $Modtime: 21/10/04 10:36 $
// $Revision: 1 $
// $Workfile: MsgRecorder.h $
//
// *****************************************************************************

#ifndef __MsgRecorder_h_
#define __MsgRecorder_h_

#include "ASN_Types.h"
class AgentServerMsgMgr;

// =============================================================================
/** @class  MsgRecorder
    @brief  MsgRecorder
*/
// Created: APE 2004-10-20
// =============================================================================
class MsgRecorder
{
private:
    class Msg
    {
        public:
             Msg();
             Msg(  ASN1PEREncodeBuffer& asnPEREncodeBuffer, int nTime, int nOffset );
             Msg( MIL_MOSContextID nContext, ASN1PEREncodeBuffer& asnPEREncodeBuffer, int nTime, int nOffset );
            ~Msg();

            int GetOffset() const;
            int GetTime() const;

            void SendSelf( AgentServerMsgMgr& msgManager );
            
//            void ReadArchive( MT_InputArchive_ABC& archive );
//            void WriteArchive( MT_OutputArchive_ABC& archive ) const;

        private:
            bool bContext_;
            MIL_MOSContextID nContext_;
            ASN1OCTET* pMsg_;
            int nMsgLength_;
            int nTime_;
            int nOffset_;
    };

    typedef std::vector< Msg* >         T_MsgVector;
    typedef T_MsgVector::iterator       IT_MsgVector;
    typedef T_MsgVector::const_iterator CIT_MsgVector;

public:
    enum E_State
    {
        eStopped,
        eRecording,
        ePlaying
    };

public:
    //! @name Constructors/Destructor
    //@{
     MsgRecorder( AgentServerMsgMgr& msgManager );
    ~MsgRecorder();
    //@}

    //! @name Operations
    //@{
    E_State GetState() const;

    void OnNewMsg( int nType, ASN1PEREncodeBuffer& asnPEREncodeBuffer );
    void OnNewMsgWithContext( int nType, MIL_MOSContextID nContext, ASN1PEREncodeBuffer& asnPEREncodeBuffer );

    void Play();
    void Record();
    void Stop();
    void Clear();

    void OnTimeTick( int nTime );

//    void ReadArchive( MT_InputArchive_ABC& archive );
//    void WriteArchive( MT_OutputArchive_ABC& archive ) const;
    //@}

private:
    //! @name Helpers
    //@{
    MsgRecorder( const MsgRecorder& );
    MsgRecorder& operator=( const MsgRecorder& );
    //@}

private:
    //! @name Member data
    //@{
    AgentServerMsgMgr& msgManager_;

    E_State nState_;
    int nTimeStart_;

    T_MsgVector messages_;
    //@}
};

#include "MsgRecorder.inl"

#endif // __MsgRecorder_h_
