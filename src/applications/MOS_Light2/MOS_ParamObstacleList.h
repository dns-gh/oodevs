// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: APE 2004-05-18 $
// $Archive: /MVW_v10/Build/SDK/MOS_Light2/src/MOS_ParamObstacleList.h $
// $Author: Ape $
// $Modtime: 23/09/04 17:35 $
// $Revision: 4 $
// $Workfile: MOS_ParamObstacleList.h $
//
// *****************************************************************************

#ifndef __MOS_ParamObstacleList_h_
#define __MOS_ParamObstacleList_h_

#ifdef __GNUG__
#   pragma interface
#endif

#include "MOS_ASN_Types.h"
#include "MOS_Param_ABC.h"

class MOS_ParamObstacle;
class MOS_ParamListView;


// =============================================================================
/** @class  MOS_ParamObstacleList
    @brief  MOS_ParamObstacleList
    @par    Using example
    @code
    MOS_ParamObstacleList;
    @endcode
*/
// Created: APE 2004-05-18
// =============================================================================
class MOS_ParamObstacleList : public QVBox, public MOS_Param_ABC
{
    Q_OBJECT;
    MT_COPYNOTALLOWED( MOS_ParamObstacleList );
    friend class MOS_GLTool;

public:
    //! @name Constructors/Destructor
    //@{
     MOS_ParamObstacleList( ASN1T_ListMissionGenObject& asnObjectList, const std::string strLabel, const std::string strMenuText, QWidget* pParent );
    ~MOS_ParamObstacleList();
    //@}

    //! @name Operations
    //@{
    void Draw               ();
    void FillRemotePopupMenu( QPopupMenu& popupMenu, const MOS_ActionContext& context );
    bool CheckValidity      ();
    void WriteMsg           ( std::stringstream& strMsg );
    //@}
    
private:
    void Copy( ASN1T_MissionGenObject& src, ASN1T_MissionGenObject& dest );

private slots:
    void OnSelectionChanged  ( QListViewItem* pItem );
    void OnRequestPopup      ( QListViewItem* pItem, const QPoint& pos );
    void OnNewObstacle       ();
    void OnDeleteSelectedItem();
    void OnClearList         ();

private:
    typedef std::vector< ASN1T_CoordUTM* >                     T_ASNUMTCoordPrtVector;
    typedef T_ASNUMTCoordPrtVector::iterator                    IT_ASNUMTCoordPrtVector;
    typedef std::pair< ASN1T_MissionGenObject*, T_PointVector > T_Item;
    typedef MT_ValuedListViewItem< T_Item >                     T_ListItem;

private:
    //! @name Member data
    //@{
    ASN1T_ListMissionGenObject& asnObjectList_;
    ASN1T_MissionGenObject*     pObstacles_;
    T_ASNUMTCoordPrtVector      asnUMTCoordPtrList_;
    
    MOS_ParamListView*          pListView_;
    QListViewItem*              pSelectedItem_;

    MOS_ParamObstacle*          pObstacleEditor_;

    QPopupMenu*                 pPopupMenu_;
    //@}
};

#   include "MOS_ParamObstacleList.inl"

#endif // __MOS_ParamObstacleList_h_
