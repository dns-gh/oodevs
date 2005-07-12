//*****************************************************************************
//
// $Created: NLD 2003-01-29 $
// $Archive: /MVW_v10/Build/SDK/MOS_Light/src/MOS_DynaObjectManager.h $
// $Author: Nld $
// $Modtime: 7/03/05 13:27 $
// $Revision: 5 $
// $Workfile: MOS_DynaObjectManager.h $
//
//*****************************************************************************

#ifndef __MOS_DynaObjectManager_h_
#define __MOS_DynaObjectManager_h_

#include "MOS_Types.h"

class MOS_DynaObject;
class MOS_DynaObject_ListView;
class QListView;

//=============================================================================
// Created: NLD 2003-01-29
//=============================================================================
class MOS_DynaObjectManager
{
    MT_COPYNOTALLOWED( MOS_DynaObjectManager );

public:
    //-------------------------------------------------------------------------
    /** @name Types */
    //-------------------------------------------------------------------------
    //@{
    typedef std::map< uint, MOS_DynaObject* >   T_DynaObjectMap;
    typedef T_DynaObjectMap::iterator                       IT_DynaObjectMap;
    typedef T_DynaObjectMap::const_iterator                 CIT_DynaObjectMap;
    //@}

public:
    MOS_DynaObjectManager();
    virtual ~MOS_DynaObjectManager();

    //-------------------------------------------------------------------------
    /** @name Init */
    //-------------------------------------------------------------------------
    //@{
    void Initialize();
    void Terminate ();
    //@}

    //-------------------------------------------------------------------------
    /** @name Main */
    //-------------------------------------------------------------------------
    //@{
    void RegisterDynaObject  ( MOS_DynaObject& object );
    void UnregisterDynaObject( MOS_DynaObject& object );
    void RemoveAllDynaObjects();

    MOS_DynaObject* FindDynaObject( uint nID ) const;     
    const T_DynaObjectMap& GetObjects() const;
    //@}
    
    //-------------------------------------------------------------------------
    /** @name GUI                                                            */
    //-------------------------------------------------------------------------
    //@{
    QListView* CreateListView( QWidget* pParent );
    //@}


    //-------------------------------------------------------------------------
    /** @name Draw */
    //-------------------------------------------------------------------------
    //@{
    void Draw();
    //@}

private:
    T_DynaObjectMap dynaObjectMap_;
    MOS_DynaObject_ListView* pListView_;
};


#ifdef MOS_USE_INLINE
#   include "MOS_DynaObjectManager.inl"
#endif

#endif // __MOS_DynaObjectManager_h_
