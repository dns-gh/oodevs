//*****************************************************************************
//
// $Created: FBD 03-01-17 $
// $Archive: /MVW_v10/Build/SDK/MOS_Light/src/MOS_ValueList.h $
// $Author: Nld $
// $Modtime: 10/09/04 11:40 $
// $Revision: 13 $
// $Workfile: MOS_ValueList.h $
//
//*****************************************************************************

#ifndef __MOS_ValueList_h_
#define __MOS_ValueList_h_


#include "MOS_Types.h"
#include "MOS_Attr_Def.h"
#include "MOS_Value.h"
#include "MOS_MissionTool_ABC.h"

#include <QFrame.h>
#include <QLayout.h>

class MOS_Agent;
class MOS_Value;
class MOS_Value;
class MOS_SubValue;
class MOS_Mission_ABC;
class QWidget;

//*****************************************************************************
// Created: FBD 03-01-17
//*****************************************************************************
class MOS_ValueList : public QFrame
                    , public MOS_MissionTool_ABC
{
    MT_COPYNOTALLOWED( MOS_ValueList );

    
private: 
    typedef std::vector< MOS_Value* >             T_ValuePtrVector;
    typedef const T_ValuePtrVector                   CT_ValuePtrVector;
    typedef T_ValuePtrVector::iterator               IT_ValuePtrVector;
    typedef T_ValuePtrVector::const_iterator         CIT_ValuePtrVector;
    typedef T_ValuePtrVector::reverse_iterator       RIT_ValuePtrVector;

public:
    explicit MOS_ValueList( QWidget* pParent, std::string sName, int nWidth = 2, int nHeight = 3 ) ;
    virtual ~MOS_ValueList();


    //-------------------------------------------------------------------------
    /** @name Init*/
    //-------------------------------------------------------------------------
    //@{
    void Initialize();
    void Terminate();
    //@}

    //-------------------------------------------------------------------------
    /** @name Create*/
    //-------------------------------------------------------------------------
    //@{
    //MOS_Value*          CreateValue( E_TypeValue nTypeValue, std::string sName, int nTool, MT_Float rValue = 0.0 );
    
    template< class T >
    MOS_Value* CreateValue( T& value, std::string sName, int nTool, MT_Float rValue = 0.0 )
    {
        MOS_Value* pValue = new MOS_Value( value, this, sName, nTool );
        pValue->SetValueWidget( rValue );
        RegisterValue( *pValue );
        pGridLayout_->addWidget( pValue, nCurHeight_, nCurWidth_ );
    
        SetNextPosition();
        return pValue;
    }

    void  DeleteValue( MOS_Value& value );
    void  RegisterValue( MOS_Value& value );
    void  UnregisterValue( MOS_Value& value );
    void  SetNextPosition();
    void  SetName( const std::string& sName );
    //@}

    //-------------------------------------------------------------------------
    /** @name DIN*/
    //-------------------------------------------------------------------------
    //@{
    void WriteMsg               ( std::string& sParam );
    void FillRandomParameters   ();
    void ReloadParameters       ( MT_InputArchive_ABC& archive );
    void ResaveParameters       ( MT_OutputArchive_ABC& archive );

    //@}

    //-------------------------------------------------------------------------
    /** @name Accesors*/
    //-------------------------------------------------------------------------
    //@{
    void       SetWidgetTab( QWidget* pWidget );
    void       SetMission( MOS_Mission_ABC* pMission );
    void       NotifyAgentHasMoved( MOS_Agent& agent, const MT_Vector2D& vPos );
    void       SetAgent( MOS_Agent* pAgent );
    MOS_Agent* GetAgent() const;

    //@}

protected:
    int                 nWidth_;
    int                 nHeight_;
    int                 nCurWidth_;
    int                 nCurHeight_;

    T_ValuePtrVector    valueVector_;

    QGridLayout*        pGridLayout_;
    MOS_Agent*          pAgent_;
    std::string         sName_;

};

#ifdef MOS_USE_INLINE
#	include "MOS_ValueList.inl"
#endif


#endif // __MOS_ValueList_h_