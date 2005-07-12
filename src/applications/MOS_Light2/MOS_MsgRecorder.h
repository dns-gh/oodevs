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
// $Archive: /MVW_v10/Build/SDK/MOS_Light2/src/MOS_MsgRecorder.h $
// $Author: Ape $
// $Modtime: 21/10/04 10:36 $
// $Revision: 1 $
// $Workfile: MOS_MsgRecorder.h $
//
// *****************************************************************************

#ifndef __MOS_MsgRecorder_h_
#define __MOS_MsgRecorder_h_

#ifdef __GNUG__
#   pragma interface
#endif

#include "MOS_ASN_Types.h"
class MOS_AgentServerMsgMgr;

// =============================================================================
/** @class  MOS_MsgRecorder
    @brief  MOS_MsgRecorder
    @par    Using example
    @code
    MOS_MsgRecorder;
    @endcode
*/
// Created: APE 2004-10-20
// =============================================================================
class MOS_MsgRecorder
{
    MT_COPYNOTALLOWED( MOS_MsgRecorder );

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

            void SendSelf( MOS_AgentServerMsgMgr& msgManager );
            
            void ReadArchive( MT_InputArchive_ABC& archive );
            void WriteArchive( MT_OutputArchive_ABC& archive ) const;

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
     MOS_MsgRecorder( MOS_AgentServerMsgMgr& msgManager );
    ~MOS_MsgRecorder();
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

    void ReadArchive( MT_InputArchive_ABC& archive );
    void WriteArchive( MT_OutputArchive_ABC& archive ) const;
    //@}

    //! @name Accessors
    //@{
    //@}

    //! @name Modifiers
    //@{
    //@}

    //! @name Operators
    //@{
    //@}

private:
    //! @name Helpers
    //@{
    //@}

private:
    //! @name Member data
    //@{
    MOS_AgentServerMsgMgr& msgManager_;

    E_State nState_;
    int nTimeStart_;

    T_MsgVector messages_;
    //@}
};

#include "MOS_MsgRecorder.inl"

#endif // __MOS_MsgRecorder_h_
