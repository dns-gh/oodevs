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
    enum ObjectMagicActionAck_ErrorCode;
}

namespace flood
{
    class FloodModel_ABC;
}

namespace xml
{
    class xistream;
}

class MIL_Army_ABC;
class TER_Localisation;
class MIL_Object_ABC;
class MIL_ObjectBuilder_ABC;
class MIL_ObjectFactory;
class MIL_UrbanObject_ABC;

// =============================================================================
// @class  MIL_ObjectManager
// Created: JVT 2004-08-03
// =============================================================================
class MIL_ObjectManager : private boost::noncopyable
{
public:
    explicit MIL_ObjectManager( MIL_ObjectFactory& factory );
    virtual ~MIL_ObjectManager();

    //! @name CheckPoints
    //@{
    BOOST_SERIALIZATION_SPLIT_MEMBER()

    void load( MIL_CheckPointInArchive&, const unsigned int );
    void save( MIL_CheckPointOutArchive&, const unsigned int ) const;
    template< typename Archive > friend  void save_construct_data( Archive& archive, const MIL_ObjectManager* role, const unsigned int /*version*/ );
    template< typename Archive > friend  void load_construct_data( Archive& archive, MIL_ObjectManager* role, const unsigned int /*version*/ );
    //@}

    //! @name Operations
    //@{
    void ProcessEvents();
    void UpdateStates( const flood::FloodModel_ABC& floodModel );
    unsigned long Count() const;

    MIL_Object_ABC& CreateObject( xml::xistream& xis, MIL_Army_ABC* army );
    MIL_Object_ABC* CreateObject( const std::string& type, MIL_Army_ABC* army, const TER_Localisation& localisation );
    MIL_Object_ABC* CreateObject( const std::string& type, MIL_Army_ABC* army, const TER_Localisation& localisation, unsigned int forcedId );
    MIL_Object_ABC* CreateObject( MIL_Army_ABC* army, const std::string& type, const TER_Localisation* pLocalisation,
                                  sword::ObstacleType_DemolitionTargetType obstacleType, unsigned int externalIdentifier, const std::string& name, double density );
    MIL_Object_ABC* CreateObject( MIL_Army_ABC* army, const MIL_ObjectBuilder_ABC& builder );
    MIL_Object_ABC* CreateUrbanObject( xml::xistream& xis, MIL_UrbanObject_ABC* parent );

    void WriteODB( xml::xostream& xos ) const;
    void ReadUrbanState( xml::xistream& xis );
    MIL_Object_ABC* Find( unsigned int nID ) const;
    const std::set< MIL_Object_ABC* >& GetUniversalObjects() const;
    //@}

    //! @name Network
    //@{
    void FinalizeObjects( const flood::FloodModel_ABC& floodModel );
    void SendCreation();
    void SendFullState();
    void OnReceiveObjectMagicAction( const sword::ObjectMagicAction& asnMsg, unsigned int nCtx, const tools::Resolver< MIL_Army_ABC >& armies,
                                     const flood::FloodModel_ABC& floodModel );
    void OnReceiveChangeResourceLinks( const sword::MagicAction& message, unsigned int nCtx );
    //@}

private:
    //! @name Types
    //@{
    typedef std::map< unsigned int, MIL_Object_ABC* > T_ObjectMap;
    typedef T_ObjectMap::iterator                    IT_ObjectMap;
    typedef T_ObjectMap::const_iterator             CIT_ObjectMap;

    typedef std::set< MIL_Object_ABC* > T_ObjectSet;
    //@}

private:
    //! @name Helpers
    //@{
    void RegisterObject( MIL_Object_ABC* pObject );
    void UpdateCapacity( const std::string& capacity, xml::xistream& xis, MIL_Object_ABC& object );
    //@}

private:
    //! @name Tools
    //@{
    sword::ObjectMagicActionAck_ErrorCode CreateObject( const sword::MissionParameters& asn, const tools::Resolver< MIL_Army_ABC >& armies,
                                                        const flood::FloodModel_ABC& floodModel );
    //@}

private:
    //! @name
    //@{
    T_ObjectMap objects_;
    T_ObjectSet universalObjects_;
    unsigned int nbObjects_;
    MIL_ObjectFactory& factory_;
    //@}
};

BOOST_CLASS_EXPORT_KEY( MIL_ObjectManager )

#include "MIL_ObjectManager.inl"

#endif // __MIL_ObjectManager_h_
