//*****************************************************************************
//
// $Created: AGN 03-07-21 $
// $Archive: /MVW_v10/Build/SDK/MOS_Light/src/MOS_ObjectKnowledge_ListView.h $
// $Author: Nld $
// $Modtime: 10/09/04 11:40 $
// $Revision: 3 $
// $Workfile: MOS_ObjectKnowledge_ListView.h $
//
//*****************************************************************************

#ifndef __MOS_ObjectKnowledge_ListView_h_
#define __MOS_ObjectKnowledge_ListView_h_

#include <QListView.h>

class QWidget;
class MOS_ObjectKnowledge;
class MOS_Team;

//*****************************************************************************
// Created: AGN 03-07-21
//*****************************************************************************
class MOS_ObjectKnowledge_ListView
: public QListView
{
public:
    explicit MOS_ObjectKnowledge_ListView( QWidget* pParent );
    virtual ~MOS_ObjectKnowledge_ListView();

    void AddKnowledge   ( MOS_ObjectKnowledge& object );
    void RemoveKnowledge( MOS_ObjectKnowledge& object );

    void SetTeam( MOS_Team* pTeam );

private:
    MOS_Team*      pTeam_;
};

#ifdef MOS_USE_INLINE
#	include "MOS_ObjectKnowledge_ListView.inl"
#endif


#endif // __MOS_ObjectKnowledge_ListView_h_