//*****************************************************************************
//
// $Created: NLD 2003-01-29 $
// $Archive: /MVW_v10/Build/SDK/MOS_Light2/src/MOS_DynaObjectManager.h $
// $Author: Ape $
// $Modtime: 30/08/04 16:37 $
// $Revision: 4 $
// $Workfile: MOS_DynaObjectManager.h $
//
//*****************************************************************************

#ifndef __MOS_DynaObjectManager_h_
#define __MOS_DynaObjectManager_h_

class MOS_DynaObject;


// =============================================================================
/** @class  MOS_DynaObjectManager
    @brief  Manages the dynamic objects.
*/
// Created: APE 2004-07-19
// =============================================================================
class MOS_DynaObjectManager
{
    MT_COPYNOTALLOWED( MOS_DynaObjectManager );
    friend class MOS_GLTool;

public:
    //-------------------------------------------------------------------------
    /** @name Types */
    //-------------------------------------------------------------------------
    //@{
    typedef std::map< uint, MOS_DynaObject* >       T_DynaObjectMap;
    typedef T_DynaObjectMap::iterator               IT_DynaObjectMap;
    typedef T_DynaObjectMap::const_iterator         CIT_DynaObjectMap;
    typedef T_DynaObjectMap::const_reverse_iterator RCIT_DynaObjectMap;
    //@}

public:
     MOS_DynaObjectManager();
    ~MOS_DynaObjectManager();

    //-------------------------------------------------------------------------
    /** @name Main */
    //-------------------------------------------------------------------------
    //@{
    void RegisterDynaObject  ( MOS_DynaObject& object );
    void UnregisterDynaObject( MOS_DynaObject& object );
    void DeleteAllDynaObjects();

    MOS_DynaObject* FindDynaObject( uint nID ) const;
    const T_DynaObjectMap& GetDynaObjects() const;
    //@}

    void ReadODB( MT_XXmlInputArchive& archive );
    void WriteODB( MT_XXmlOutputArchive& archive );

private:
    T_DynaObjectMap dynaObjectMap_;
};


#ifdef MOS_USE_INLINE
#   include "MOS_DynaObjectManager.inl"
#endif

#endif // __MOS_DynaObjectManager_h_
