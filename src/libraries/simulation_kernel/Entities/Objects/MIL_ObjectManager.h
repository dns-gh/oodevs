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

#include "game_asn/Simulation.h"
#include "tools/Resolver.h"

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
    
    void load( MIL_CheckPointInArchive&, const uint );
    void save( MIL_CheckPointOutArchive&, const uint ) const;
    //@}
    
    //! @name Operations
    //@{
    void ProcessEvents();
    void UpdateStates ();

    //@TODO MGD return reference
    MIL_Object_ABC&  CreateObject( xml::xistream& xis, MIL_Army_ABC& army ); 
    MIL_Object_ABC*  CreateObject( const std::string& type, MIL_Army_ABC& army, const TER_Localisation& localisation );
    MIL_Object_ABC*  CreateObject( MIL_Army_ABC& army, const std::string& type, const TER_Localisation* pLocalisation, ASN1T_EnumDemolitionTargetType obstacleType );
    MIL_Object_ABC*  CreateObject( MIL_Army_ABC& army, const MIL_ObjectBuilder_ABC& builder );
    MIL_Object_ABC*             Find( uint nID ) const;
    const MIL_ObjectType_ABC&   FindType( const std::string& type ) const;
    //@}

    //! @name Network
    //@{
    void OnReceiveMsgObjectMagicAction( const ASN1T_MsgObjectMagicAction& asnMsg, uint nCtx, const tools::Resolver< MIL_Army_ABC >& armies );
    //@}

    //! @name 
    //@{
    void RegisterObject( MIL_Object_ABC& object );
    //@}

private:
    //! @name Types
    //@{
    typedef std::map< uint, MIL_Object_ABC* >   T_ObjectMap;
    typedef T_ObjectMap::iterator               IT_ObjectMap;
    typedef T_ObjectMap::const_iterator         CIT_ObjectMap;
    //@}

private:
    //! @name Tools
    //@{
    ASN1T_EnumObjectErrorCode CreateObject( const ASN1T_MagicActionCreateObject& asn, const tools::Resolver< MIL_Army_ABC >& armies );    
    //@}

private:
    //! @name 
    //@{
    T_ObjectMap                         objects_;
    std::auto_ptr< MIL_ObjectFactory >  builder_;
    //@}
};

BOOST_CLASS_EXPORT_KEY( MIL_ObjectManager )

#include "MIL_ObjectManager.inl"

#endif // __MIL_ObjectManager_h_
