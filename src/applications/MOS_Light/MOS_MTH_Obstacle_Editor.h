//*****************************************************************************
//
// $Created: AGN 03-08-26 $
// $Archive: /MVW_v10/Build/SDK/MOS_Light/src/MOS_MTH_Obstacle_Editor.h $
// $Author: Nld $
// $Modtime: 10/09/04 11:40 $
// $Revision: 4 $
// $Workfile: MOS_MTH_Obstacle_Editor.h $
//
//*****************************************************************************

#ifndef __MOS_MTH_Obstacle_Editor_h_
#define __MOS_MTH_Obstacle_Editor_h_

#include <qframe.h>

class MOS_MTH_Obstacle_ListViewItem;

class MOS_MTH_Localisation;
class QComboBox;

//*****************************************************************************
// Created: AGN 03-08-26
//*****************************************************************************
class MOS_MTH_Obstacle_Editor
: public QFrame
{
    Q_OBJECT

public:
    explicit MOS_MTH_Obstacle_Editor( QWidget* pParent );
    virtual ~MOS_MTH_Obstacle_Editor();

    void SetEditedItem( MOS_MTH_Obstacle_ListViewItem* pEditedItem );

private slots:
    void SlotOk();
    void SlotCancel();

private:
    QComboBox* pTypeObstacleGUI_;
    
    MOS_MTH_Localisation* pLocalisation_;

    QComboBox* pUrgenceGUI_;
    QComboBox* pPreliminaireGUI_;
    QComboBox* pPrioriteGUI_;
    QSpinBox*  pIDPlanifieGUI_;

    MOS_MTH_Obstacle_ListViewItem* pEditedItem_;
};

#ifdef MOS_USE_INLINE
#	include "MOS_MTH_Obstacle_Editor.inl"
#endif


#endif // __MOS_MTH_Obstacle_Editor_h_