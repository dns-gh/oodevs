// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ColorStrategy_ABC_h_
#define __ColorStrategy_ABC_h_

namespace kernel
{
    class Entity_ABC;
    class Agent_ABC;
    class Automat_ABC;
    class Drawing_ABC;
    class Formation_ABC;
    class Ghost_ABC;
    class Object_ABC;
    class Pathfind_ABC;
    class Population_ABC;
    class Inhabitant_ABC;
    class Knowledge_ABC;
    class TacticalLine_ABC;
    class Team_ABC;
    class UrbanObject_ABC;
}

namespace gui
{

// =============================================================================
/** @class  ColorStrategy_ABC
    @brief  Color selection strategy definition
*/
// Created: AGE 2006-03-17
// =============================================================================
class ColorStrategy_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             ColorStrategy_ABC() {};
    virtual ~ColorStrategy_ABC() {};
    //@}

    //! @name Operations
    //@{
    virtual void SetAlpha( float alpha ) = 0;
    virtual void SelectColor( const kernel::Agent_ABC& agent ) = 0;
    virtual void SelectColor( const kernel::Automat_ABC& automat ) = 0;
    virtual void SelectColor( const kernel::Formation_ABC& formation ) = 0;
    virtual void SelectColor( const kernel::Object_ABC& object ) = 0;
    virtual void SelectColor( const kernel::Population_ABC& population ) = 0;
    virtual void SelectColor( const kernel::Inhabitant_ABC& inhabitant ) = 0;
    virtual void SelectColor( const kernel::Knowledge_ABC& knowledge ) = 0;
    virtual void SelectColor( const kernel::TacticalLine_ABC& line ) = 0;
    virtual void SelectColor( const kernel::Team_ABC& team ) = 0;
    virtual void SelectColor( const kernel::Drawing_ABC& drawing ) = 0;
    virtual void SelectColor( const kernel::UrbanObject_ABC& object ) = 0;
    virtual void SelectColor( const kernel::Ghost_ABC& ghost ) = 0;
    virtual void SelectColor( const kernel::Pathfind_ABC& pathfind ) = 0;
    virtual QColor FindColor( const kernel::Entity_ABC& entity ) = 0;
    virtual QColor FindColorWithModifiers( const kernel::Entity_ABC& entity ) = 0;
    virtual QColor FindBaseColor( const kernel::Entity_ABC& entity ) = 0;
    virtual QColor FindColor( const kernel::Knowledge_ABC& knowledge ) = 0;
    //@}
};

}

#endif // __ColorStrategy_ABC_h_
