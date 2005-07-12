// *****************************************************************************
//
// $Created: NLD 2003-08-05 $
// $Archive: /MVW_v10/Build/SDK/MOS_Light/src/MOS_DynaObject_Editor.h $
// $Author: Nld $
// $Modtime: 10/09/04 11:40 $
// $Revision: 2 $
// $Workfile: MOS_DynaObject_Editor.h $
//
// *****************************************************************************

#ifndef __MOS_DynaObject_Editor_h_
#define __MOS_DynaObject_Editor_h_

#include "MOS_Types.h"

#include <qframe.h>

class QSpinBox;
class QCheckBox;
class MOS_DynaObject;

// =============================================================================
// Created: NLD 2003-08-05
// =============================================================================
class MOS_DynaObject_Editor : public QFrame
{
    Q_OBJECT
    MT_COPYNOTALLOWED( MOS_DynaObject_Editor )

public:
     MOS_DynaObject_Editor( QWidget* pParent ); 
    ~MOS_DynaObject_Editor(); 

    //! @name Main
    //@{
    void SetObject( const MOS_DynaObject* pObject );
    //@}

private slots:

    //! @name Slots
    //@{
    void SlotValidate();
    void SlotCancel  ();
    //@}


private:
    const MOS_DynaObject* pObject_;
    
    QSpinBox*   pPrctConstructionSpinBox_;
    QSpinBox*   pPrctValorizationSpinBox_;
    QSpinBox*   pPrctBypassCreationSpinBox_;
    QCheckBox*  pPreparedCheckBox_;
};

#ifdef MOS_USE_INLINE
#   include "MOS_DynaObject_Editor.inl"
#endif

#endif // __MOS_DynaObject_Editor_h_
