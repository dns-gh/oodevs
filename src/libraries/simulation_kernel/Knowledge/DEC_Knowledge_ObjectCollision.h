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
#include "MT_Tools/MT_Vector2D.h"

class MIL_Object_ABC;
class MIL_Agent_ABC;

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
             DEC_Knowledge_ObjectCollision( const MIL_Agent_ABC& agentColliding, MIL_Object_ABC& object );
    explicit DEC_Knowledge_ObjectCollision( MIL_Object_ABC& object );
             DEC_Knowledge_ObjectCollision ();
    virtual ~DEC_Knowledge_ObjectCollision();
    //@}

    //! @name CheckPoints
    //@{
    template< typename Archive > void serialize( Archive&, const unsigned int );
    //@}

    //! @name Operations
    //@{
    void Prepare();
    void Update( const MT_Vector2D& vPosition );
    bool Clean();
    //@}

    //! @name Accessors
    //@{
    MIL_Object_ABC& GetObject() const;
    const MIL_Agent_ABC* GetAgentColliding() const;
    const MT_Vector2D& GetPosition() const;
    bool IsValid() const;

    template< typename T > const T* GetAttribute() const;
    //@}

private:
    //! @name Data Members
    //@{
    const MIL_Agent_ABC* pAgentColliding_;
    MIL_Object_ABC* pObject_;
    MT_Vector2D vPosition_;
    bool bIsValid_;
    //@}
};

BOOST_CLASS_EXPORT_KEY( DEC_Knowledge_ObjectCollision )

// =============================================================================
// Implementation
// =============================================================================

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_ObjectCollision::GetAttribute
// Created: NLD 2004-05-04
// -----------------------------------------------------------------------------
template< typename T > const T* DEC_Knowledge_ObjectCollision::GetAttribute() const
{
    return pObject_ ? pObject_->RetrieveAttribute< T >() : 0;
}

#endif // __DEC_Knowledge_ObjectCollision_h_
