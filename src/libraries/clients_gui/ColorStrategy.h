// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ColorStrategy_h_
#define __ColorStrategy_h_

#include "ColorStrategy_ABC.h"
#include <tools/ElementObserver_ABC.h>
#include <boost/ptr_container/ptr_vector.hpp>

namespace kernel
{
    class Controllers;
    class UrbanObject_ABC;
    class Drawing_ABC;
}

namespace gui
{
    class ColorModifier_ABC;
    class GLView_ABC;
}

namespace gui
{
// =============================================================================
/** @class  ColorStrategy
    @brief  ColorStrategy
*/
// Created: AGE 2006-03-17
// =============================================================================
class ColorStrategy : public ColorStrategy_ABC
                    , public tools::Observer_ABC
                    , public tools::ElementObserver_ABC< kernel::Team_ABC >
{
public:
    //! @name Constructors/Destructor
    //@{
             ColorStrategy( kernel::Controllers& controllers, GLView_ABC& tools, ColorModifier_ABC& colorController );
    virtual ~ColorStrategy();
    //@}

    //! @name Operations
    //@{
    virtual void SetAlpha( float alpha );
    virtual void SelectColor( const kernel::Agent_ABC& agent );
    virtual void SelectColor( const kernel::Automat_ABC& automat );
    virtual void SelectColor( const kernel::Formation_ABC& formation );
    virtual void SelectColor( const kernel::Object_ABC& object );
    virtual void SelectColor( const kernel::Population_ABC& population );
    virtual void SelectColor( const kernel::Inhabitant_ABC& inhabitant );
    virtual void SelectColor( const kernel::Knowledge_ABC& knowledge );
    virtual void SelectColor( const kernel::TacticalLine_ABC& line );
    virtual void SelectColor( const kernel::Team_ABC& team );
    virtual void SelectColor( const kernel::Drawing_ABC& drawing );
    virtual void SelectColor( const kernel::UrbanObject_ABC& object );
    virtual void SelectColor( const kernel::Ghost_ABC& ghost );
    virtual void SelectColor( const kernel::Pathfind_ABC& pathfind );
    virtual QColor FindColor( const kernel::Entity_ABC& entity );
    virtual QColor FindColorWithModifiers( const kernel::Entity_ABC& entity );
    virtual QColor FindBaseColor( const kernel::Entity_ABC& entity );
    virtual QColor FindTeamColor( const kernel::Entity_ABC& team );
    virtual QColor FindColor( const kernel::Knowledge_ABC& knowledge );
    //@}

    //! @name Modifiers
    //@{
    void Add( std::unique_ptr< ColorModifier_ABC > modifier );
    //@}

private:
    //! @name Copy/Assignment
    //@{
    ColorStrategy( const ColorStrategy& );            //!< Copy constructor
    ColorStrategy& operator=( const ColorStrategy& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    virtual void NotifyCreated( const kernel::Team_ABC& );
    virtual void NotifyUpdated( const kernel::Team_ABC& );
    virtual void NotifyDeleted( const kernel::Team_ABC& );

    void InitializeColors();

    QColor RandomColor() const;
    QColor KnowledgeColor( const QColor& base ) const;

    void ApplyColor( const QColor& color ) const;
    void ApplyColor( const QColor& color, float alpha ) const;
    void Process( const kernel::Entity_ABC& entity );
    void Process( const kernel::Entity_ABC& entity, QColor color );
    QColor ApplyModifiers( const kernel::Entity_ABC& entity, const QColor& color );
    float  ApplyModifiers( const kernel::Entity_ABC& entity, float alpha );
    //@}

    //! @name Types
    //@{
    typedef boost::ptr_vector< ColorModifier_ABC >                                T_Modifiers;
    typedef std::vector< QColor >                                                 T_Colors;
    typedef std::map< const kernel::Entity_ABC*, std::pair< QColor, T_Colors* > > T_TeamColors;
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controllers& controllers_;
    GLView_ABC& tools_;
    ColorModifier_ABC& colorController_;
    T_Modifiers modifiers_;
    T_TeamColors teamColors_;
    T_Colors friendlyAvailable_;
    T_Colors enemyAvailable_;
    T_Colors neutralAvailable_;
    float alpha_;
    //@}
};

}

#endif // __ColorStrategy_h_
