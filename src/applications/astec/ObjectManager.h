//*****************************************************************************
//
// $Created: NLD 2003-01-29 $
// $Archive: /MVW_v10/Build/SDK/Light2/src/ObjectManager.h $
// $Author: Ape $
// $Modtime: 30/08/04 16:37 $
// $Revision: 4 $
// $Workfile: ObjectManager.h $
//
//*****************************************************************************

#ifndef __ObjectManager_h_
#define __ObjectManager_h_

#include "Types.h"
#include "ASN_Types.h"

class Object_ABC;

// =============================================================================
/** @class  ObjectManager
    @brief  Manages the dynamic objects.
*/
// Created: APE 2004-07-19
// =============================================================================
class ObjectManager
{
    MT_COPYNOTALLOWED( ObjectManager );
    friend class GLTool;

public:
    //-------------------------------------------------------------------------
    /** @name Types */
    //-------------------------------------------------------------------------
    //@{
    typedef std::map< uint, Object_ABC* >   T_ObjectMap;
    typedef T_ObjectMap::iterator               IT_ObjectMap;
    typedef T_ObjectMap::const_iterator         CIT_ObjectMap;
    typedef T_ObjectMap::const_reverse_iterator RCIT_ObjectMap;
    //@}

public:
     ObjectManager();
    ~ObjectManager();

    //-------------------------------------------------------------------------
    /** @name Main */
    //-------------------------------------------------------------------------
    //@{
    void CreateObject( const ASN1T_MsgObjectCreation& asnMsg );
    void RegisterObject  ( Object_ABC& object );
    void UnregisterObject( Object_ABC& object );
    void DeleteAllObjects();

    Object_ABC*    FindObject( uint nID ) const;
    const T_ObjectMap& GetObjects() const;
    //@}

    void ReadODB( InputArchive& archive );
    void WriteODB( MT_XXmlOutputArchive& archive );

private:
    T_ObjectMap ObjectMap_;
};

#   include "ObjectManager.inl"

#endif // __ObjectManager_h_
