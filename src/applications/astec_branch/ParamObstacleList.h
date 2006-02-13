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
// $Archive: /MVW_v10/Build/SDK/Light2/src/ParamObstacleList.h $
// $Author: Ape $
// $Modtime: 23/09/04 17:35 $
// $Revision: 4 $
// $Workfile: ParamObstacleList.h $
//
// *****************************************************************************

#ifndef __ParamObstacleList_h_
#define __ParamObstacleList_h_

#ifdef __GNUG__
#   pragma interface
#endif

#include "ASN_Types.h"
#include "Param_ABC.h"

class ParamObstacle;
class ParamListView;


// =============================================================================
/** @class  ParamObstacleList
    @brief  ParamObstacleList
    @par    Using example
    @code
    ParamObstacleList;
    @endcode
*/
// Created: APE 2004-05-18
// =============================================================================
class ParamObstacleList : public QVBox, public Param_ABC
{
    Q_OBJECT;
    MT_COPYNOTALLOWED( ParamObstacleList );
    friend class GLTool;

public:
    //! @name Constructors/Destructor
    //@{
     ParamObstacleList( ASN1T_ListMissionGenObject& asnObjectList, const std::string strLabel, const std::string strMenuText, QWidget* pParent, bool bOptional );
    ~ParamObstacleList();
    //@}

    //! @name Operations
    //@{
    void Draw               ();
    void FillRemotePopupMenu( QPopupMenu& popupMenu, const ActionContext& context );
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
    typedef std::vector< ASN1T_CoordUTM* >                      T_ASNUMTCoordPrtVector;
    typedef T_ASNUMTCoordPrtVector::iterator                    IT_ASNUMTCoordPrtVector;
    typedef std::pair< ASN1T_MissionGenObject*, T_PointVector > T_Item;
    typedef MT_ValuedListViewItem< T_Item >                     T_ListItem;

private:
    //! @name Member data
    //@{
    ASN1T_ListMissionGenObject& asnObjectList_;
    ASN1T_MissionGenObject*     pObstacles_;
    T_ASNUMTCoordPrtVector      asnUMTCoordPtrList_;
    
    ParamListView*          pListView_;
    QListViewItem*              pSelectedItem_;

    ParamObstacle*          pObstacleEditor_;

    QPopupMenu*                 pPopupMenu_;
    //@}
};

#   include "ParamObstacleList.inl"

#endif // __ParamObstacleList_h_
