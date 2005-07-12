//*****************************************************************************
//
// $Created: FBD 03-01-24 $
// $Archive: /MVW_v10/Build/SDK/MOS_Light/src/MOS_ValueButtonList.h $
// $Author: Nld $
// $Modtime: 19/07/04 17:18 $
// $Revision: 10 $
// $Workfile: MOS_ValueButtonList.h $
//
//*****************************************************************************

#ifndef __MOS_ValueButtonList_h_
#define __MOS_ValueButtonList_h_

#include "MOS_Types.h"
#include "MOS_ValueButton.h"
#include "MOS_MissionTool_ABC.h"


#include <QButtonGroup.h>
#include <QLayout.h>

//*****************************************************************************
// Created: FBD 03-01-24
//*****************************************************************************
template <class T>
class MOS_ValueButtonList : public QButtonGroup
                          , public MOS_MissionTool_ABC
{
    MT_COPYNOTALLOWED( MOS_ValueButtonList );
private: 
    typedef std::vector< MOS_ValueButton<T>* >          T_valueItemPtrVector;
    typedef const T_valueItemPtrVector                  CT_valueItemPtrVector;
    typedef typename T_valueItemPtrVector::iterator              IT_valueItemPtrVector;
	typedef typename T_valueItemPtrVector::const_iterator        CIT_valueItemPtrVector;
    typedef typename T_valueItemPtrVector::reverse_iterator      RIT_valueItemPtrVector;


public:
    MOS_ValueButtonList( T& output, QWidget* pParent, const std::string& sName, int nWidth, int nHeight, bool bExclusive = true );
    ~MOS_ValueButtonList();


    //-------------------------------------------------------------------------
    /** @name Init*/
    //-------------------------------------------------------------------------
    //@{
    void Initialize();
    void Terminate();
    //@}

 
    //-------------------------------------------------------------------------
    /** @name Accessors*/
    //-------------------------------------------------------------------------
    //@{
    MOS_ValueButton<T>&  CreateValue    ( const std::string& sName, T value, bool bSelected_ = false );
    void                 DeleteValue    ( MOS_ValueButton<T>& valueItem );
    void                 RegisterValue  ( MOS_ValueButton<T>& valueItem );
    void                 UnregisterValue( MOS_ValueButton<T>& valueItem );
    void                 SetNextPosition();
    void                 SetWidgetTab( QWidget* pWidget );
    void                 SetMission( MOS_Mission_ABC* pMission );
    void                 NotifyAgentHasMoved( MOS_Agent& agent, const MT_Vector2D& vPos );
    void                 SetAgent( MOS_Agent* pAgent );
    //@}

    //-------------------------------------------------------------------------
    /** @name DIN */
    //-------------------------------------------------------------------------
    //@{
    void WriteMsg( std::string& sParam );
    void FillRandomParameters();
    void ReloadParameters( MT_InputArchive_ABC& archive );
    void ResaveParameters( MT_OutputArchive_ABC& archive );
    
    //@}

private:
    T& output_;

    std::string                 sName_;
    QGridLayout*                pLayout_;
    T_valueItemPtrVector        valueItemVector_;
    bool                        bExclusive_;
    int                         nWidth_;
    int                         nHeight_;
    int                         nCurWidth_;
    int                         nCurHeight_;
};

#	include "MOS_ValueButtonList.inl"

#endif // __MOS_ValueButtonList_h_