// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef __PHY_RoleAction_MovingUnderground_h_
#define __PHY_RoleAction_MovingUnderground_h_

#include "MT_Tools/Role_ABC.h"
#include "MT_Tools/MT_Vector2D.h"
#include "MT_Tools/AlgorithmModifier_ABC.h"

namespace detection
{
    class DetectionComputer_ABC;
}

class MIL_Agent_ABC;
class MIL_Object_ABC;

// =============================================================================
/** @class  PHY_RoleAction_MovingUnderground
    @brief  PHY_RoleAction_MovingUnderground
*/
// Created: JSR 2011-06-08
// =============================================================================
class PHY_RoleAction_MovingUnderground : public tools::Role_ABC
                                       , public tools::AlgorithmModifier_ABC< detection::DetectionComputer_ABC >
                                       , private boost::noncopyable
{
public:
    //! @name Types
    //@{
    typedef PHY_RoleAction_MovingUnderground RoleInterface;
    //@}

public:
    //! @name Constructors/Destructor
    //@{
    explicit PHY_RoleAction_MovingUnderground( MIL_Agent_ABC& pion );
    virtual ~PHY_RoleAction_MovingUnderground();
    //@}

    //! @name CheckPoints
    //@{
    template< typename Archive >
    void serialize( Archive&, const unsigned int );
    //@}

    //! @name Operations
    //@{
    virtual void Update( bool bIsDead );
    virtual void Clean();
    virtual void Execute( detection::DetectionComputer_ABC& algorithm ) const;

    void InitializeUndergroundMoving( const MIL_Object_ABC& firstObject, const MIL_Object_ABC& secondObject );
    bool Run();
    bool IsUnderground() const;
    double EstimatedUndergroundTime( const MIL_Object_ABC& firstObject, const MIL_Object_ABC& secondObject ) const;
    void HideInUndergroundNetwork( const MIL_Object_ABC& object );
    void GetOutFromUndergroundNetwork();
    //@}

private:
    //! @name Helpers
    //@{
    void Reset();
    template< typename Archive >
    friend void save_construct_data( Archive& archive, const PHY_RoleAction_MovingUnderground* role, const unsigned int /*version*/ );
    template< typename Archive >
    friend void load_construct_data( Archive& archive, PHY_RoleAction_MovingUnderground* role, const unsigned int /*version*/ );
    //@}

private:
    //! @name Member data
    //@{
    MIL_Agent_ABC& pion_;
    double transferTime_;
    MT_Vector2D firstPosition_;
    MT_Vector2D secondPosition_;
    //@}
};

BOOST_CLASS_EXPORT_KEY( PHY_RoleAction_MovingUnderground )

#endif // __PHY_RoleAction_MovingUnderground_h_
