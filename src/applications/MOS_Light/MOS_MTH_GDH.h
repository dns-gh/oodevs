//****************************************************************************
//
// $Created:  NLD 2002-01-03 $
// $Archive: /MVW_v10/Build/SDK/MOS_Light/src/MOS_MTH_GDH.h $
// $Author: Nld $
// $Modtime: 10/09/04 11:40 $
// $Revision: 8 $
// $Workfile: MOS_MTH_GDH.h $
//
//*****************************************************************************

#ifndef __MOS_MTH_GDH_h_
#define __MOS_MTH_GDH_h_

#include "MOS_Types.h"
#include "MOS_Attr_Def.h"
#include "MOS_MissionTool_ABC.h"
#include "MOS_ASN_Types.h"

#include <QFrame.h>
#include <QWidget.h>
#include <QSlider.h>
#include <QSpinBox.h>
#include <QDial.h>
#include <QLabel.h>
#include <QCheckBox.h>

class MOS_Agent;
class QPushButton;
class QVBoxLayout;
class QDateTimeEdit;

//=============================================================================
// Created:  NLD 2002-01-03 
//=============================================================================

class MOS_MTH_GDH : public QFrame, public MOS_MissionTool_ABC
{
    Q_OBJECT
    MT_COPYNOTALLOWED( MOS_MTH_GDH );

public:
     MOS_MTH_GDH( ASN1T_GDH& asnGDH, QWidget* pParent, const std::string& sName );
    ~MOS_MTH_GDH();

    //-------------------------------------------------------------------------
    /** @name Init */
    //-------------------------------------------------------------------------
    //@{
    virtual void Initialize();
    virtual void Terminate ();
    //@}
 
    //-------------------------------------------------------------------------
    /** @name Accessors*/
    //-------------------------------------------------------------------------
    //@{
    void  SetAgent( MOS_Agent* pAgent );
    //@}

    //-------------------------------------------------------------------------
    /** @name Agent*/
    //-------------------------------------------------------------------------
    //@{
    virtual void SetMission  ( MOS_Mission_ABC* pMission );
    virtual void SetWidgetTab( QWidget* pWidget );
    virtual void NotifyAgentHasMoved( MOS_Agent& agent, const MT_Vector2D& vPos );
    //@}
    //-------------------------------------------------------------------------
    /** @name */
    //-------------------------------------------------------------------------
    //@{
    virtual void WriteMsg( std::string& sParam );
    virtual void FillRandomParameters();
    virtual void ReloadParameters( MT_InputArchive_ABC& archive );
    virtual void ResaveParameters( MT_OutputArchive_ABC& archive );
    //@}

public slots:
    void SlotUseGDHToogled( bool bToggled );

private:
    ASN1T_GDH&     asnGDH_;
    std::string    sName_;
    QDateTimeEdit* pDateTimeEdit_;
    QCheckBox*     pDontUseGdhCheckBox_;
};

#ifdef MOS_USE_INLINE
#   include "MOS_MTH_GDH.inl"
#endif

#endif // __MOS_MTH_GDH_h_
