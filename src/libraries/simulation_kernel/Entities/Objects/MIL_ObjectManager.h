// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Objects/MIL_ObjectManager.h $
// $Author: Nld $
// $Modtime: 10/06/05 10:18 $
// $Revision: 6 $
// $Workfile: MIL_ObjectManager.h $
//
// *****************************************************************************

#ifndef __MIL_ObjectManager_h_
#define __MIL_ObjectManager_h_

#include "MIL.h"
#include <tools/Resolver.h>

namespace sword
{
    enum ObstacleType_DemolitionTargetType;
    class MissionParameters;
    class MagicAction;
    class ObjectMagicAction;
    class UrbanMagicAction;
    enum ObjectMagicActionAck_ErrorCode;
}

namespace urban
{
    class TerrainObject_ABC;
}

namespace xml
{
    class xistream;
}

class MIL_Army_ABC;
class MIL_Army;
class TER_Localisation;
class MIL_Object_ABC;
class MIL_ObjectType_ABC;
class MIL_ObjectBuilder_ABC;
class MIL_ObjectFactory;
class UrbanObjectWrapper;

// =============================================================================
// @class  MIL_ObjectManager
// Created: JVT 2004-08-03
// =============================================================================
class MIL_ObjectManager : private boost::noncopyable
{
public:
             MIL_ObjectManager();
    virtual ~MIL_ObjectManager();

    //! @name CheckPoints
    //@{
    BOOST_SERIALIZATION_SPLIT_MEMBER()

    void load( MIL_CheckPointInArchive&, const unsigned int );
    void save( MIL_CheckPointOutArchive&, const unsigned int ) const;
    //@}

    //! @name Operations
    //@{
    void ProcessEvents();
    void UpdateStates ();

    //@TODO MGD return reference
    MIL_Object_ABC&     CreateObject( xml::xistream& xis, MIL_Army_ABC& army );
    MIL_Object_ABC*     CreateObject( const std::string& type, MIL_Army_ABC& army, const TER_Localisation& localisation );
    MIL_Object_ABC*     CreateDistantObject( const std::string& type, MIL_Army_ABC& army, const TER_Localisation& localisation, const std::string& name );
    MIL_Object_ABC*     CreateObject( MIL_Army_ABC& army, const std::string& type, const TER_Localisation* pLocalisation, sword::ObstacleType_DemolitionTargetType obstacleType );
    MIL_Object_ABC*     CreateObject( MIL_Army_ABC& army, const MIL_ObjectBuilder_ABC& builder );
    MIL_Object_ABC*     CreateUrbanObject( const urban::TerrainObject_ABC& object );
    void                UpdateCapacity( const std::string& capacity, xml::xistream& xis, MIL_Object_ABC& object );
    void                ReadUrbanState( xml::xistream& xis );
    UrbanObjectWrapper* FindUrbanWrapper( unsigned int nID );
    MIL_Object_ABC*     Find( unsigned int nID ) const;
    const MIL_ObjectType_ABC&   FindType( const std::string& type ) const;
    //@}

    //! @name Network
    //@{
    void SendCreation();
    void SendFullState();
    void OnReceiveObjectMagicAction( const sword::ObjectMagicAction& asnMsg, unsigned int nCtx, const tools::Resolver< MIL_Army_ABC >& armies );
    void OnReceiveUrbanMagicAction( const sword::UrbanMagicAction& asnMsg, unsigned int nCtx );
    void OnReceiveChangeResourceLinks( const sword::MagicAction& message, unsigned int nCtx );
    //@}

private:
    //! @name Types
    //@{
    typedef std::map< unsigned int, MIL_Object_ABC* > T_ObjectMap;
    typedef T_ObjectMap::iterator                    IT_ObjectMap;
    typedef T_ObjectMap::const_iterator             CIT_ObjectMap;
    //@}

private:
    //! @name Helpers
    //@{
    void RegisterObject( MIL_Object_ABC* pObject );
    void RegisterDistantObject( MIL_Object_ABC* pObject );
    //@}

private:
    //! @name Tools
    //@{
    sword::ObjectMagicActionAck_ErrorCode CreateObject( const sword::MissionParameters& asn, const tools::Resolver< MIL_Army_ABC >& armies );
    //@}

private:
    //! @name
    //@{
    T_ObjectMap objects_;
    std::auto_ptr< MIL_ObjectFactory >  builder_;
    std::vector< unsigned int > urbanIds_;
    //@}
};

BOOST_CLASS_EXPORT_KEY( MIL_ObjectManager )

#include "MIL_ObjectManager.inl"

#endif // __MIL_ObjectManager_h_
