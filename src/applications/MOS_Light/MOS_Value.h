//****************************************************************************
//
// $Created:  NLD 2002-01-03 $
// $Archive: /MVW_v10/Build/SDK/MOS_Light/src/MOS_Value.h $
// $Author: Nld $
// $Modtime: 10/09/04 11:40 $
// $Revision: 22 $
// $Workfile: MOS_Value.h $
//
//*****************************************************************************

#ifndef __MOS_Value_h_
#define __MOS_Value_h_

#include "MOS_Types.h"
#include "MOS_Attr_Def.h"
#include "MOS_ASN_Types.h"

#include <QFrame.h>
#include <QWidget.h>
#include <QSlider.h>
#include <QSpinBox.h>
#include <QDial.h>
#include <QLabel.h>
#include <QCheckBox.h>

class MOS_Agent;
class MOS_LifeBar;
class QPushButton;
class QVBoxLayout;
class QLCDNumber;

enum E_TypeValue
{
    eTypeValueInt,
    eTypeValueFloat,
    eTypeValueBool
};

#define MOSToolValueTitle    0x00001
#define MOSToolValueSpeedo   0x00002
#define MOSToolValueLifeBar  0x00004
#define MOSToolValueSpin     0x00008
#define MOSToolValueText     0x00010
#define MOSToolValueSlide    0x00020
#define MOSToolValueCheck    0x00040
#define MOSToolValueModif    0x00080
#define MOSToolValueTime     0x00100

//=============================================================================
// Created:  NLD 2002-01-03 
//=============================================================================

class MOS_Value : public QFrame
{
    Q_OBJECT
    MT_COPYNOTALLOWED( MOS_Value );

public:
    MOS_Value( ASN1INT& value , QWidget* pParent, const std::string& sName, int nTools );
    MOS_Value( ASN1BOOL& value, QWidget* pParent, const std::string& sName, int nTools );
    MOS_Value( ASN1REAL& value, QWidget* pParent, const std::string& sName, int nTools );
	MOS_Value( E_TypeValue nTypeValue, QWidget* pParent, const std::string& sName, int nTools );
    ~MOS_Value();

    //-------------------------------------------------------------------------
    /** @name Init */
    //-------------------------------------------------------------------------
    //@{
    virtual void Initialize();
    virtual void Terminate ();
    virtual void Refresh();
    //@}
 

    //-------------------------------------------------------------------------
    /** @name Accessors*/
    //-------------------------------------------------------------------------
    //@{
    int         GetId() const;

    void SetId( int nType );

    void                CreateButtons();
    void                SetAgent( MOS_Agent* pAgent );
    MOS_Agent*          GetAgent() const;
    const std::string&  GetName() const;
    QDial*              GetSpeedo() const;


    void                UpdateValueMin( MT_Float rValueMin );
    void                UpdateValueMax( MT_Float rValueMax );
    void                UpdateValue   ( MT_Float rValue );

    void                SetValueWidget( MT_Float rValueWidget );
    MT_Float            GetValueWidget() const;
    void                SetValueMaxWidget( MT_Float rValueMaxWidget );
    MT_Float            GetValueMaxWidget() const;
    void                SetValueMinWidget( MT_Float rValueMinWidget );
    void                SetModifiedWidget( bool bModifiedWidget );
    bool                IsModifiedWidget() const;


    void                SetValueNetwork( MT_Float rValueNetwork );
    MT_Float            GetValueNetwork() const;
    void                SetValueMinNetwork( MT_Float rValueMinNetwork );
    MT_Float            GetValueMinNetwork() const;
    void                SetValueMaxNetwork( MT_Float rValueMaxNetwork );
    MT_Float            GetValueMaxNetwork() const;
    void                SetModifiedNetwork( bool bModifiedNetwork );
    bool                IsModifiedNetwork() const;

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

protected:
    //-------------------------------------------------------------------------
    /** @name */
    //-------------------------------------------------------------------------
    //@{
    bool setIndicator( QString & indicator, int progress, int totalSteps );
    //@}

protected slots:

    //-------------------------------------------------------------------------
    /** @name */
    //-------------------------------------------------------------------------
    //@{
    void SlotSetValueSlider     ( int nValue );
    void SlotSetValueSpinBox    ( int nValue );
    void SlotSetValueSpeedo     ( int nValue );
    void SlotCheckBoxToggled    ( bool bState );
    void SlotButtonPressed      ();
    //@}
    
protected:
    E_TypeValue   nTypeValue_;
    QVBoxLayout*  pMainLayout_;

    bool          bModifiedWidget_;
    MT_Float      rValueMinWidget_;
    MT_Float      rValueWidget_;
    MT_Float      rValueMaxWidget_;

    bool          bModifiedNetwork_;
    MT_Float      rValueMinNetwork_;
    MT_Float      rValueNetwork_;
    MT_Float      rValueMaxNetwork_;
    
    
    MOS_Agent*    pAgent_;

    int nType_;

private:
    //-------------------------------------------------------------------------
    /** @name */
    //-------------------------------------------------------------------------
    //@{
    void Construct();
    //@}    

private:
    int           nTools_;
    std::string   sName_;
    bool          bModifiable_;
    MOS_LifeBar*  pLifeBar_;
    QPushButton*  pPushButton_;
    QSlider*      pSlider_;
    QSpinBox*     pSpinBox_;
    QDial*        pSpeedo_;
    QLabel*       pLabelVal_;
    QCheckBox*    pCheckBox_;
    QLCDNumber*   pTime_;

    // ASN - $$$ faire une classe template
    ASN1INT*  pAsnIntValue_;
    ASN1BOOL* pAsnBoolValue_;
    ASN1REAL* pAsnRealValue_;
};

#ifdef MOS_USE_INLINE
#   include "MOS_Value.inl"
#endif

#endif // __MOS_Value_h_
