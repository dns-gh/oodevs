// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ListParameter_h_
#define __ListParameter_h_

#include "Param_ABC.h"
#include "clients_gui/ShapeHandler_ABC.h"
#include "actions/ParameterList.h"
#include "clients_gui/Tools.h"
#include "clients_gui/ValuedListItem.h"
#include "MissionInterface_ABC.h"
#include "InterfaceBuilder_ABC.h"

class kernel::ContextMenu;

namespace kernel
{
    class OrderParameter;
    class CoordinateConverter_ABC;
    class Location_ABC;
}

namespace gui
{
    class ParametersLayer;
}

namespace actions
{
    class Parameter_ABC;

    namespace gui
    {

// =============================================================================
/** @class  ListParameterBase
    @brief  ListParameterBase
*/
// Created: MMC 2011-12-15
// =============================================================================
class ListParameterBase : public Param_ABC
{
    Q_OBJECT

public:
    //! @name Constructors/Destructor
    //@{
    ListParameterBase( const InterfaceBuilder_ABC& builder, const kernel::OrderParameter& parameter )
        : Param_ABC( builder.GetParentObject(), builder.GetParamInterface(), parameter ) {}

    virtual ~ListParameterBase() {}
    //@}

    //! @name Operations
    //@{
    virtual bool IsSelected( Param_ABC* param ) = 0;
    virtual bool IsPotential( Param_ABC* param ) = 0;
    virtual Param_ABC* CreateElement() = 0;
    virtual void AddElement( Param_ABC& param ) = 0;
    virtual void EnableCreation( bool enabled ) = 0;
    //@}

protected slots:
    //! @name slots (can't mock a template class)
    //@{
    virtual void OnCreate() = 0;
    virtual void OnDeleteSelectedItem() = 0;
    virtual void OnClear() = 0;
    virtual void OnRequestPopup( Q3ListViewItem* item, const QPoint& pos ) = 0;
    virtual void OnSelectionChanged( Q3ListViewItem* item ) = 0;
    virtual void TurnHeaderBlack() = 0;
    //@}
};


// =============================================================================
/** @class  ListParameter
    @brief  ListParameter
*/
// Created: SBO 2007-04-26
// =============================================================================

template< typename ConcreteElement >
class ListParameter : public ListParameterBase
{

public:
    //! @name Constructors/Destructor
    //@{*
             ListParameter( const InterfaceBuilder_ABC& builder, const kernel::OrderParameter& parameter );
    virtual ~ListParameter();
    //@}

    //! @name Drawable_ABC implementation
    //@{
    virtual void Draw( const geometry::Point2f& point, const kernel::Viewport_ABC& viewport, const kernel::GlTools_ABC& tools ) const;
    //@}

    //! @name ListParameterBase implementation
    //@{
    virtual void CreateInternalMenu( kernel::ContextMenu& mainMenu );
    virtual bool IsSelected( Param_ABC* param );
    virtual bool IsPotential( Param_ABC* param );
    virtual void AddElement( Param_ABC& param );
    //@}

    //! @name Param_ABC implementation
    //@{
    virtual QWidget* BuildInterface( QWidget* parent );
    virtual void CommitTo( actions::ParameterContainer_ABC& action ) const;
    virtual Param_ABC* CreateElement();
    //@}

protected:
    //! @name Operations
    //@{
    virtual void EnableCreation( bool enabled );
    virtual bool InternalCheckValidity() const;
    unsigned int Count() const;
    void Select( const Param_ABC& param );
    virtual void DeleteElement( Param_ABC& param );
    virtual void SetName( const QString& name );
    //@}

private:
    // Slots implementation (can't mock a template class)

    //! @name Param_ABC implementation
    //@{
    virtual void OnMenuClick();
    //@}

    //! @name ListParameterBase implementation
    //@{
    virtual void OnCreate();
    virtual void OnDeleteSelectedItem();
    virtual void OnClear();
    virtual void OnRequestPopup( Q3ListViewItem* item, const QPoint& pos );
    virtual void OnSelectionChanged( Q3ListViewItem* item );
    virtual void TurnHeaderBlack();
    //@}

private:
    //! @name Helpers
    //@{
    void CreatePotential();
    bool CommitChildrenTo( actions::ParameterContainer_ABC& parent ) const;
    void DeleteItem( Q3ListViewItem* item );
    void Clear();
    bool Invalid();
    //@}

    //! @name Types
    //@{
    typedef std::map< const Param_ABC*, QWidget* > T_Widgets;
    //@}

protected :
    //! @name Member data
    //@{
    unsigned int count_;
    Param_ABC* potential_;
    //@}

private:
    //! @name Member data
    //@{
    const InterfaceBuilder_ABC& builder_;
    kernel::ActionController& controller_;
    Q3ListView* list_;
    Q3ListViewItem* selected_;
    T_Widgets widgets_;
    unsigned int min_;
    unsigned int max_;
    bool createEnabled_;
    //@}

};

#include "ListParameter.inl"

    }
}

#endif // __ListParameter_h_
