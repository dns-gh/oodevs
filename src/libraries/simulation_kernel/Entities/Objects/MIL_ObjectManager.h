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
#include <vector>
#include <map>

namespace sword
{
    enum ObstacleType_DemolitionTargetType;
    class MissionParameters;
    class MagicAction;
    class ObjectMagicAction;
    enum ObjectMagicActionAck_ErrorCode;
}

namespace propagation
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
             MIL_ObjectManager( MIL_ObjectFactory& factory );
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
    void Clean();
    void ProcessEvents();
    void UpdateStates( const propagation::FloodModel_ABC& floodModel );
    unsigned long Count() const;

    MIL_Object_ABC& CreateObject( xml::xistream& xis, MIL_Army_ABC* army );
    MIL_Object_ABC* CreateObject( const std::string& type, MIL_Army_ABC* army, const TER_Localisation& localisation );
    MIL_Object_ABC* CreateObject( const std::string& type, MIL_Army_ABC* army, const TER_Localisation& localisation, unsigned int forcedId );
    MIL_Object_ABC* CreateObject( MIL_Army_ABC* army, const std::string& type, const TER_Localisation* pLocalisation,
                                  bool activated, unsigned int externalIdentifier, const std::string& name, double density );
    MIL_Object_ABC* CreateObject( MIL_Army_ABC* army, const MIL_ObjectBuilder_ABC& builder );
    MIL_Object_ABC* CreateUrbanObject( xml::xistream& xis, MIL_UrbanObject_ABC* parent );

    void WriteODB( xml::xostream& xos ) const;
    void ReadUrbanState( xml::xistream& xis );
    MIL_Object_ABC* Find( unsigned int nID ) const;
    void VisitUniversalObjects( const std::function< void( MIL_Object_ABC& ) >& visitor ) const;
    void VisitUrbanObjects( const std::function< void( const MIL_UrbanObject_ABC& ) >& functor ) const;
    const std::map< unsigned int, MIL_Object_ABC* >& GetObjects() const;
    //@}

    //! @name Network
    //@{
    void FinalizeObjects( const propagation::FloodModel_ABC& floodModel );
    void SendCreation();
    void SendFullState();
    void OnReceiveObjectMagicAction( const sword::ObjectMagicAction& asn,
                                     sword::ObjectMagicActionAck& ack,
                                     const tools::Resolver< MIL_Army_ABC >& armies,
                                     const propagation::FloodModel_ABC& floodModel );
    void OnReceiveChangeResourceLinks( const sword::MagicAction& message );
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
    MIL_Object_ABC* CreateObject( const sword::MissionParameters& asn, const tools::Resolver< MIL_Army_ABC >& armies,
                                  const propagation::FloodModel_ABC& floodModel );
    //@}

private:
    //! @name
    //@{
    unsigned int nbObjects_;
    MIL_ObjectFactory& factory_;
    std::map< unsigned int, MIL_Object_ABC* > objects_;
    std::vector< MIL_Object_ABC* > universalObjects_;
    //@}
};

BOOST_CLASS_EXPORT_KEY( MIL_ObjectManager )

#endif // __MIL_ObjectManager_h_
