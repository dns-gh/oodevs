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
#include "actions/ParameterList.h"
#include "InterfaceBuilder_ABC.h"
#include "clients_kernel/SafePointer.h"

Q_DECLARE_METATYPE( actions::gui::Param_ABC* )

class kernel::ContextMenu;

namespace kernel
{
    class OrderParameter;
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
public:
    //! @name Types
    //@{
    enum E_Role {
        ParamRole = Qt::UserRole,
        IdRole    = Qt::UserRole + 1
    };
    //@}

    Q_OBJECT

public:
    //! @name Constructors/Destructor
    //@{
    ListParameterBase( const InterfaceBuilder_ABC& builder, const kernel::OrderParameter& parameter )
        : Param_ABC( builder, parameter ) {}

    virtual ~ListParameterBase() {}
    //@}

    //! @name Operations
    //@{
    virtual bool IsSelected( Param_ABC* param ) const = 0;
    virtual bool IsPotential( Param_ABC* param ) const = 0;
    virtual bool CanCreate() const = 0;
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
    virtual void OnRequestPopup( const QPoint& pos ) = 0;
    virtual void OnSelectionChanged( const QItemSelection& newSelection , const QItemSelection& oldSelection ) = 0;
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
    virtual void Draw( const geometry::Point2f& point, const ::gui::Viewport_ABC& viewport, ::gui::GLView_ABC& tools ) const;
    //@}

    //! @name ListParameterBase implementation
    //@{
    virtual void CreateInternalMenu( kernel::ContextMenu& mainMenu );
    virtual bool IsSelected( Param_ABC* param ) const;
    virtual bool IsPotential( Param_ABC* param ) const;
    virtual bool CanCreate() const;
    virtual void AddElement( Param_ABC& param );
    //@}

    //! @name Param_ABC implementation
    //@{
    virtual QWidget* BuildInterface( const QString& objectName, QWidget* parent );
    virtual void CommitTo( actions::ParameterContainer_ABC& action ) const;
    virtual Param_ABC* CreateElement();
    virtual void SetEntity( const kernel::Entity_ABC* entity );
    virtual bool HasParameter( const Param_ABC& param ) const;
    //@}

    //! @name ParameterVisitor_ABC implementation
    //@{
    virtual void Visit( const actions::Parameter_ABC& param );
    //@}

protected:
    //! @name Operations
    //@{
    virtual void EnableCreation( bool enabled );
    virtual bool InternalCheckValidity() const;
    unsigned int Count() const;
    void Select( const Param_ABC& param );
    virtual void DeleteElement( Param_ABC& param );
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
    virtual void OnRequestPopup( const QPoint& pos );
    virtual void OnSelectionChanged( const QItemSelection& newSelection , const QItemSelection& oldSelection );
    //@}

private:
    //! @name Helpers
    //@{
    void CreatePotential();
    QString CreateNextNameAndId();
    bool CommitChildrenTo( actions::ParameterContainer_ABC& parent ) const;
    void DeleteItem( QStandardItem* item );
    void Clear();
    //@}

    //! @name Types
    //@{
    typedef std::map< const Param_ABC*, QWidget* > T_Widgets;
    //@}

protected :
    //! @name Member data
    //@{
    unsigned int    count_;
    Param_ABC*      potential_;
    //@}

private:
    //! @name Member data
    //@{
    const InterfaceBuilder_ABC& builder_;
    QTreeView*                  list_;
    QStandardItem*              selected_;
    QStandardItemModel          model_;
    T_Widgets                   widgets_;
    unsigned int                min_;
    unsigned int                max_;
    bool                        createEnabled_;
    kernel::SafePointer< kernel::Entity_ABC > entity_;
    //@}

};

#include "ListParameter.inl"

    }
}

#endif // __ListParameter_h_
