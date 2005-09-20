//*****************************************************************************
//
// $Created: NLD 2003-01-29 $
// $Archive: /MVW_v10/Build/SDK/MOS_Light2/src/MOS_ObjectManager.h $
// $Author: Ape $
// $Modtime: 30/08/04 16:37 $
// $Revision: 4 $
// $Workfile: MOS_ObjectManager.h $
//
//*****************************************************************************

#ifndef __MOS_ObjectManager_h_
#define __MOS_ObjectManager_h_

#include "MOS_Types.h"
class MOS_Object_ABC;

// =============================================================================
/** @class  MOS_ObjectManager
    @brief  Manages the dynamic objects.
*/
// Created: APE 2004-07-19
// =============================================================================
class MOS_ObjectManager
{
    MT_COPYNOTALLOWED( MOS_ObjectManager );
    friend class MOS_GLTool;

public:
    //-------------------------------------------------------------------------
    /** @name Types */
    //-------------------------------------------------------------------------
    //@{
    typedef std::map< uint, MOS_Object_ABC* >   T_ObjectMap;
    typedef T_ObjectMap::iterator               IT_ObjectMap;
    typedef T_ObjectMap::const_iterator         CIT_ObjectMap;
    typedef T_ObjectMap::const_reverse_iterator RCIT_ObjectMap;
    //@}

public:
     MOS_ObjectManager();
    ~MOS_ObjectManager();

    //-------------------------------------------------------------------------
    /** @name Main */
    //-------------------------------------------------------------------------
    //@{
    void RegisterObject  ( MOS_Object_ABC& object );
    void UnregisterObject( MOS_Object_ABC& object );
    void DeleteAllObjects();

    MOS_Object_ABC*    FindObject( uint nID ) const;
    const T_ObjectMap& GetObjects() const;
    //@}

    void ReadODB( MOS_InputArchive& archive );
    void WriteODB( MT_XXmlOutputArchive& archive );

private:
    T_ObjectMap ObjectMap_;
};


#ifdef MOS_USE_INLINE
#   include "MOS_ObjectManager.inl"
#endif

#endif // __MOS_ObjectManager_h_
