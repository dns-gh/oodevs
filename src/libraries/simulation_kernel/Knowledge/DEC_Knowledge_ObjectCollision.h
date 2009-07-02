// *****************************************************************************
//
// $Created: NLD 2004-03-11 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Knowledge/DEC_Knowledge_ObjectCollision.h $
// $Author: Jvt $
// $Modtime: 14/04/05 12:33 $
// $Revision: 6 $
// $Workfile: DEC_Knowledge_ObjectCollision.h $
//
// *****************************************************************************

#ifndef __DEC_Knowledge_ObjectCollision_h_
#define __DEC_Knowledge_ObjectCollision_h_

#include "DEC_Knowledge_ABC.h"

class MIL_Object_ABC;
class MIL_AgentPion;

// =============================================================================
/** @class  DEC_Knowledge_ObjectCollision
    @brief  This class represents the Collision of an object 
            An object can be
                - perceived : the object is seen by the sensors
                - detected  : the object 
*/
// Created: NLD 2004-03-11
// =============================================================================
class DEC_Knowledge_ObjectCollision : public DEC_Knowledge_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             DEC_Knowledge_ObjectCollision( const MIL_AgentPion& agentColliding, MIL_Object_ABC& object );
             DEC_Knowledge_ObjectCollision ();
    virtual ~DEC_Knowledge_ObjectCollision();
    //@}

    //! @name CheckPoints
    //@{
    template< typename Archive > void serialize( Archive&, const uint );
    //@}
    
    //! @name Operations
    //@{
    void Prepare();
    void Update ( const MT_Vector2D& vPosition );
    bool Clean  ();
    //@}

    //! @name Accessors
    //@{
          MIL_Object_ABC&      GetObject        () const;
    const MIL_AgentPion&       GetAgentColliding() const;
    const MT_Vector2D&         GetPosition      () const;
          bool                 IsValid          () const;
    //@}

private:
    const MIL_AgentPion*       pAgentColliding_;
          MIL_Object_ABC*      pObject_;
          MT_Vector2D          vPosition_;
          bool                 bIsValid_;
};

#endif // __DEC_Knowledge_ObjectCollision_h_
