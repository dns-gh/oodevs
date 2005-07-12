//****************************************************************************
//
// $Created:  NLD 2002-01-03 $
// $Archive: /MVW_v10/Build/SDK/MOS_Light2/src/MOS_SurrenderDialog.h $
// $Author: Age $
// $Modtime: 7/03/05 19:40 $
// $Revision: 1 $
// $Workfile: MOS_SurrenderDialog.h $
//
//*****************************************************************************

#ifndef __MOS_SurrenderDialog_h_
#define __MOS_SurrenderDialog_h_

#include "MOS_Types.h"
#include <QDialog.h>

class QComboBox;
class MOS_Agent;
class MOS_DynaObject;

//=============================================================================
// Created:  NLD 2002-01-03 
//=============================================================================
class MOS_SurrenderDialog : public QDialog
{
    Q_OBJECT
    MT_COPYNOTALLOWED( MOS_SurrenderDialog );

public:
     MOS_SurrenderDialog( QWidget* pParent = 0 );
    ~MOS_SurrenderDialog();

    void SetAgent( const MOS_Agent& agent );

private slots:
    //-------------------------------------------------------------------------
    /** @name Main methods */
    //-------------------------------------------------------------------------
    //@{
    void Validate();
    void Reject  ();
    //@}

private:
    //! @name Types
    //@{
    typedef std::map< uint, const MOS_DynaObject* > T_ObjectIDMap;
    typedef T_ObjectIDMap::const_iterator           CIT_ObjectIDMap;
    //@}
   
private:
    bool bInitialized_;
    const MOS_Agent* pAgent_;

    QComboBox* pPrisonerCampComboBox_;
    T_ObjectIDMap prisonerCampComboBoxIDs_;
};

#include "MOS_SurrenderDialog.inl"

#endif // __MOS_SurrenderDialog_h_
