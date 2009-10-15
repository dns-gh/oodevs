// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __PHY_RoleHLA_Location_h_
#define __PHY_RoleHLA_Location_h_

#include "PHY_RoleInterface_Location.h"
#include "hla/Deserializer.h"

namespace hla 
{
    class AttributeIdentifier;    
}

class MIL_AgentHLA;

// =============================================================================
/** @class  PHY_RoleHLA_Location
    @brief  HLA role location
*/
// Created: AGE 2004-11-09
// =============================================================================
class PHY_RoleHLA_Location : public PHY_RoleInterface_Location
{

public:
    //! @name Constructors/Destructor
    //@{
             PHY_RoleHLA_Location( MIL_AgentHLA& pion );
    virtual ~PHY_RoleHLA_Location();
    //@}

    //! @name Operations
    //@{
    virtual void NotifyTerrainObjectCollision    ( MIL_Object_ABC& object );

    virtual void NotifyTerrainPutInsideObject    ( MIL_Object_ABC& object );
    virtual void NotifyTerrainPutOutsideObject   ( MIL_Object_ABC& object );

    void Deserialize( const hla::AttributeIdentifier& attributeID, hla::Deserializer deserializer );

    virtual void Update( bool bIsDead ) {};
    virtual void Clean () {};

    virtual void Hide     () {};
    virtual void Show     ( const MT_Vector2D& vNewPosition ) {};
    virtual void MagicMove( const MT_Vector2D& vNewPosition ) {};
    virtual void Move     ( const MT_Vector2D& vNewPosition, const MT_Vector2D& vNewDirection, MT_Float rNewSpeed ) {};
    virtual void Follow   ( const MIL_Agent_ABC& agent ) {};
    virtual void Fly      ( MT_Float rHeight ) {};
    //@}

    //! @name Accessors
    //@{
    virtual MT_Float           GetHeight            () const;
    virtual MT_Float           GetAltitude          () const;
    virtual const MT_Vector2D& GetPosition          () const;
    virtual const MT_Vector2D& GetDirection         () const;
    virtual MIL_Agent_ABC&     GetAgent             () const;
    virtual MT_Float           GetCurrentSpeed      () const;
    virtual bool               HasDoneMagicMove     () const;
    virtual bool               HasSpeedChanged      () const { return false; }; // Position or direction or height has changed 
    virtual bool               HasLocationChanged   () const { return false; };

    bool IsValid() const;
    //@}

    //! @name Crap
    //@{
    virtual MT_Float GetCost( const MT_Vector2D& from, const MT_Vector2D& to, const TerrainData& nToTerrainType, const TerrainData& nLinkTerrainType, void* pParams ) const;
    //@}

    virtual void Serialize( HLA_UpdateFunctor& functor ) const {};

private:
    //! @name Member data
    //@{
    MIL_AgentHLA& pion_;
    MT_Vector2D   vDirection_;
    MT_Vector2D   vPosition_;
    MT_Float      rHeight_;
    MT_Float      rCurrentSpeed_;
    //@}
};

#endif // __PHY_RoleHLA_Location_h_
