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
#include "clients_kernel/Observer_ABC.h"
#include "clients_kernel/ElementObserver_ABC.h"
#include "clients_kernel/SelectionObserver_ABC.h"
#include "clients_kernel/SafePointer.h"

namespace kernel
{
    class Team_ABC;
    class GlTools_ABC;
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
                    , public kernel::Observer_ABC
                    , public kernel::ElementObserver_ABC< kernel::Team_ABC >
                    , public kernel::SelectionObserver_ABC
                    , public kernel::SelectionObserver_Base< kernel::Entity_ABC >
{

public:
    //! @name Constructors/Destructor
    //@{
             ColorStrategy( kernel::Controllers& controllers, kernel::GlTools_ABC& tools );
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
    virtual void SelectColor( const kernel::Knowledge_ABC& knowledge );
    virtual void SelectColor( const kernel::TacticalLine_ABC& line );
    virtual QColor FindColor( const kernel::Entity_ABC& entity );
    virtual QColor FindColor( const kernel::Knowledge_ABC& knowledge );
    //@}

private:
    //! @name Copy/Assignement
    //@{
    ColorStrategy( const ColorStrategy& );            //!< Copy constructor
    ColorStrategy& operator=( const ColorStrategy& ); //!< Assignement operator
    //@}

    //! @name Types 
    //@{
    typedef std::vector< QColor >                                                 T_Colors;
    typedef std::map< const kernel::Entity_ABC*, std::pair< QColor, T_Colors* > > T_TeamColors;
    //@}

    //! @name Helpers
    //@{
    virtual void NotifyCreated( const kernel::Team_ABC& );
    virtual void NotifyUpdated( const kernel::Team_ABC& );
    virtual void NotifyDeleted( const kernel::Team_ABC& );

    virtual void BeforeSelection();
    virtual void Select( const kernel::Entity_ABC& element );
    virtual void AfterSelection();

    void InitializeColors();

    QColor RandomColor() const;
    QColor SelectedColor( const QColor& base ) const;
    QColor SuperiorSelectedColor( const QColor& base ) const;
    QColor KnowledgeColor( const QColor& base ) const;
    void ApplyColor( const QColor& color ) const;
    QColor FindTeamColor( const kernel::Entity_ABC& team );

    void Process( const kernel::Entity_ABC& entity );
    QColor ApplySelectionStatus( const kernel::Entity_ABC& entity, const QColor& base );
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controllers& controllers_;
    kernel::GlTools_ABC& tools_;

    kernel::SafePointer< kernel::Entity_ABC > selectedEntity_;

    T_TeamColors teamColors_;
    T_Colors     friendlyAvailable_;
    T_Colors     enemyAvailable_;
    T_Colors     neutralAvailable_;
    float        alpha_;
    //@}
};

}

#endif // __ColorStrategy_h_
