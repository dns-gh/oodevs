//*****************************************************************************
//
// $Created: NLD 2002-07-15 $
// $Archive: /MVW_v10/Build/SDK/MOS_Light/src/MOS_LineManager.h $
// $Author: Nld $
// $Modtime: 28/04/03 11:53 $
// $Revision: 10 $
// $Workfile: MOS_LineManager.h $
//
//*****************************************************************************

#ifndef __MOS_LineManager_h_
#define __MOS_LineManager_h_

#include "MOS_Types.h"
#include "MOS_Lima.h"
#include "MOS_Limit.h"

class MOS_Limit;
class MOS_LimaListView;

//=============================================================================
// Created: NLD 2002-07-15
//=============================================================================
class MOS_LineManager
{
    MT_COPYNOTALLOWED( MOS_LineManager );

public:
    //-------------------------------------------------------------------------
    /** @name Types */
    //-------------------------------------------------------------------------
    //@{
    typedef std::set< MOS_Lima* >             T_LimaSet;
    typedef const T_LimaSet                   CT_LimaSet;
    typedef T_LimaSet::iterator               IT_LimaSet;
    typedef T_LimaSet::const_iterator         CIT_LimaSet;

    typedef std::set< MOS_LimaListView* >     T_LimaListViewSet;
    typedef const T_LimaListViewSet           CT_LimaListViewSet;
    typedef T_LimaListViewSet::iterator       IT_LimaListViewSet;
    typedef T_LimaListViewSet::const_iterator CIT_LimaListViewSet;

    typedef std::set< MOS_Limit* >              T_LimitSet;
    typedef const T_LimitSet                    CT_LimitSet;
    typedef T_LimitSet::iterator                IT_LimitSet;
    typedef T_LimitSet::const_iterator          CIT_LimitSet;

    typedef std::set< MOS_LimitListView* >     T_LimitListViewSet;
    typedef const T_LimitListViewSet           CT_LimitListViewSet;
    typedef T_LimitListViewSet::iterator       IT_LimitListViewSet;
    typedef T_LimitListViewSet::const_iterator CIT_LimitListViewSet;
    //@}

public:
     MOS_LineManager();
    ~MOS_LineManager();

    //-------------------------------------------------------------------------
    /** @name Init */
    //-------------------------------------------------------------------------
    //@{
    void Initialize();
    void Terminate ();
    //@}
    
    //-------------------------------------------------------------------------
    /** @name Limas */
    //-------------------------------------------------------------------------
    //@{
    void             FillListView               ( MOS_Lima::T_LimaItemMap& limaItemMap );
    void             RegisterLimaListView       ( MOS_LimaListView& limaListView );
    void             UnregisterLimaListView     ( MOS_LimaListView& limaListView );
    void             RegisterListViewsForLima   ( MOS_Lima& lima );
    void             RegisterLima               ( MOS_Lima& lima );
    void             UnregisterLima             ( MOS_Lima& lima );
    void             RemoveLima                 ( MIL_LimaID nLimaID );
    void             RemoveAllLimas             ();
    MOS_Lima*        FindLima                   ( MIL_LimaID nLimaID );
    CT_LimaSet&      GetLimaList                ();
    bool             UpdateLimaToSim            ();
    //@}

    //-------------------------------------------------------------------------
    /** @name Limits */
    //-------------------------------------------------------------------------
    //@{
    void             FillListView               ( MOS_Limit::T_LimitItemMap& LimitItemMap );
    void             RegisterLimitListView      ( MOS_LimitListView& LimitListView );
    void             UnregisterLimitListView    ( MOS_LimitListView& LimitListView );
    void             RegisterListViewsForLimit  ( MOS_Limit& Limit );
    void             RegisterLimit              ( MOS_Limit& lima );
    void             UnregisterLimit            ( MOS_Limit& lima );
    void             RemoveLimit                ( MIL_LimitID nLimitID );
    void             RemoveAllLimits            ();
    MOS_Limit*       FindLimit                  ( MIL_LimitID nLimitID );
    CT_LimitSet&     GetLimitList               ();
    bool             UpdateLimitToSim           ();
    //@}

    //-------------------------------------------------------------------------
    /** @name Common */
    //-------------------------------------------------------------------------
    //@{
    void RemoveAll();
    //@}


private:
    T_LimaSet               limaSet_;
    T_LimitSet              limitSet_;

    T_LimaListViewSet       limaListViewSet_;
    T_LimitListViewSet      limitListViewSet_;
    
};

#ifdef MOS_USE_INLINE
#   include "MOS_LineManager.inl"
#endif

#endif // __MOS_LineManager_h_
