//*****************************************************************************
//
// $Created: AGN 03-08-26 $
// $Archive: /MVW_v10/Build/SDK/MOS_Light/src/MOS_MTH_PointList_Editor.h $
// $Author: Nld $
// $Modtime: 10/09/04 11:40 $
// $Revision: 2 $
// $Workfile: MOS_MTH_PointList_Editor.h $
//
//*****************************************************************************

#ifndef __MOS_MTH_PointList_Editor_h_
#define __MOS_MTH_PointList_Editor_h_

#include <qwidget.h>

class MOS_MTH_Localisation_ListView;
class MOS_MTH_PointList_ListViewItem;

//*****************************************************************************
// Created: AGN 03-08-26
//*****************************************************************************
class MOS_MTH_PointList_Editor
: public QWidget
{
    Q_OBJECT

public:
    explicit MOS_MTH_PointList_Editor( QWidget* pParent );
    virtual ~MOS_MTH_PointList_Editor();

    void SetEditedItem( MOS_MTH_PointList_ListViewItem* pEditedItem );

private slots:
    void SlotOk();
    void SlotCancel();

private:
    MOS_MTH_Localisation_ListView* pLocalisation_;
    MOS_MTH_PointList_ListViewItem* pEditedItem_;
};

#ifdef MOS_USE_INLINE
#	include "MOS_MTH_PointList_Editor.inl"
#endif


#endif // __MOS_MTH_PointList_Editor_h_