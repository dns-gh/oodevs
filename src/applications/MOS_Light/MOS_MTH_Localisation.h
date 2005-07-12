//*****************************************************************************
//
// $Created: FBD 03-01-13 $
// $Archive: /MVW_v10/Build/SDK/MOS_Light/src/MOS_MTH_Localisation.h $
// $Author: Nld $
// $Modtime: 21/01/05 18:13 $
// $Revision: 8 $
// $Workfile: MOS_MTH_Localisation.h $
//
//*****************************************************************************

#ifndef __MOS_MTH_Localisation_h_
#define __MOS_MTH_Localisation_h_

#include "MOS_Types.h"
#include "MOS_PointListView.h"
#include "MOS_ASN_Types.h"

class QWidget;
class QPopupMenu;
class QListViewItem;
class MOS_Agent;
class MOS_Mission_ABC;
class QWidget;
class MOS_MTH_Localisation_ListView;

    
//*****************************************************************************
// Created: FBD 03-01-13
//*****************************************************************************
class MOS_MTH_Localisation : public QWidget, public MOS_MissionTool_ABC
{
    MT_COPYNOTALLOWED( MOS_MTH_Localisation );

public:
    //-------------------------------------------------------------------------
    /** @name Constructor/Destructor*/
    //-------------------------------------------------------------------------
    //@{
    explicit MOS_MTH_Localisation( ASN1T_Localisation& asnListPoint, QWidget* pParent, const std::string& sName );
    explicit MOS_MTH_Localisation( QWidget* pParent, const std::string& sName );
    virtual ~MOS_MTH_Localisation();
    //@}

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
    void SetMission    ( MOS_Mission_ABC* pMission );
    void SetWidgetTab  ( QWidget* pWidget );
    void SetFirstWidget( bool bFirstWidget );

    MOS_MTH_Localisation_ListView& GetListView() const;

    void                       SetLocalisationType( ASN1T_EnumTypeLocalisation nType );
    ASN1T_EnumTypeLocalisation GetLocalisationType() const;
    //@}

    //-------------------------------------------------------------------------
    /** @name Agent*/
    //-------------------------------------------------------------------------
    //@{
    void Clear   ();
    void SetAgent( MOS_Agent* pAgent );
    //@}


    //-------------------------------------------------------------------------
    /** @name DIN*/
    //-------------------------------------------------------------------------
    //@{
    void WriteMsg( std::string& sParam );

    void WriteMsg( ASN1T_Localisation& asnLocalisation, std::string& sParam );
    void FillRandomParameters( );
    void ReloadParameters( MT_InputArchive_ABC& archive );
    void ResaveParameters( MT_OutputArchive_ABC& archive );
    //@}

    //-------------------------------------------------------------------------
    /** @name Notify*/
    //-------------------------------------------------------------------------
    //@{
    void NotifyAgentHasMoved( MOS_Agent& agent, const MT_Vector2D& vPos );
    //@}
    
private:
    QComboBox*           pTypeComboBox_;

    ASN1T_Localisation*  pAsnLocalisation_;

    MOS_MTH_Localisation_ListView* pListView_;

    bool             bMovingLine_;

    MOS_Agent*       pAgent_;
    MT_Vector2D      vOldPos_;
    MOS_Mission_ABC* pMission_;
    QWidget*         pWidgetTab_;
    bool             bFirstWidget_;
};

#ifdef MOS_USE_INLINE
#	include "MOS_MTH_Localisation.inl"
#endif


#endif // __MOS_MTH_Localisation_h_