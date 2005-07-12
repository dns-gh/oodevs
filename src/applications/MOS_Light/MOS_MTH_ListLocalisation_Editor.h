//*****************************************************************************
//
// $Created: AGN 03-08-08 $
// $Archive: /MVW_v10/Build/SDK/MOS_Light/src/MOS_MTH_ListLocalisation_Editor.h $
// $Author: Nld $
// $Modtime: 10/09/04 11:40 $
// $Revision: 2 $
// $Workfile: MOS_MTH_ListLocalisation_Editor.h $
//
//*****************************************************************************

#ifndef __MOS_MTH_ListLocalisation_Editor_h_
#define __MOS_MTH_ListLocalisation_Editor_h_

#include <qframe.h>

class MOS_MTH_Localisation;
class MOS_MTH_ListLocalisation_ListViewItem;

//*****************************************************************************
// Created: AGN 03-08-08
//*****************************************************************************
class MOS_MTH_ListLocalisation_Editor
: public QFrame
{
    Q_OBJECT

public:
    explicit MOS_MTH_ListLocalisation_Editor( QWidget* pParent );
    virtual ~MOS_MTH_ListLocalisation_Editor();

    void SetEditedItem( MOS_MTH_ListLocalisation_ListViewItem* pEditedItem );

private slots:
    void SlotOk();
    void SlotCancel();

private:
    MOS_MTH_Localisation* pLocalisation_;
    MOS_MTH_ListLocalisation_ListViewItem* pEditedItem_;
};

#ifdef MOS_USE_INLINE
#	include "MOS_MTH_ListLocalisation_Editor.inl"
#endif


#endif // __MOS_MTH_ListLocalisation_Editor_h_