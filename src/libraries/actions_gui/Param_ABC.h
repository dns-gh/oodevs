// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __Param_ABC_h_
#define __Param_ABC_h_

#include "actions/ParameterVisitor_ABC.h"
#include "clients_gui/Drawable_ABC.h"
#include "clients_gui/RichGroupBox.h"
#include "clients_kernel/ContextMenu.h"
#include "clients_kernel/OrderParameter.h"
#include <tools/Observer_ABC.h>
#include <tools/Resolver.h>

namespace kernel
{
    class Controllers;
    class AgentKnowledgeConverter_ABC;
    class CoordinateConverter_ABC;
    class OrderParameter;
    class Entity_ABC;
    class Controller;
    class ObjectTypes;
    class Object_ABC;
}

namespace gui
{
    class GlTools_ABC;
    class Viewport_ABC;
}

namespace actions
{
    namespace gui
    {
        class ParamInterface_ABC;
    }
}

namespace gui
{
    class RichGroupBox;
    class ParametersLayer;
}

namespace actions
{
    class Parameter_ABC;
    class ParameterContainer_ABC;

    namespace gui
    {
        class InterfaceBuilder_ABC;
        class ListParameterBase;
        class ParamInterface_ABC;

// =============================================================================
/** @class  Param_ABC
    @brief  Param_ABC
*/
// Created: APE 2004-04-20
// =============================================================================
class Param_ABC : public QObject
                , public tools::Observer_ABC
                , public ::gui::Drawable_ABC
                , public ParameterVisitor_ABC
{
    Q_OBJECT

public:
    //! @name Constructors/Destructor
    //@{
             Param_ABC( const InterfaceBuilder_ABC& builder, const kernel::OrderParameter& parameter );
    virtual ~Param_ABC();
    //@}

public:
    //! @name Operations
    //@{
    void RegisterListener( Param_ABC& param );
    void NotifyChange();
    virtual void NotifyChanged( Param_ABC& param );
    virtual void RemoveFromController();
    virtual void RegisterIn();
    virtual void Draw( const geometry::Point2f& point, const ::gui::Viewport_ABC& viewport, ::gui::GlTools_ABC& tools ) const;
    virtual QWidget* BuildInterface( const QString& objectName, QWidget* parent );
    virtual bool CheckValidity();
    virtual bool InternalCheckValidity() const;
    virtual void CommitTo( actions::ParameterContainer_ABC& ) const = 0;
    virtual void SetEntity( const kernel::Entity_ABC* entity );

    virtual void Purge();
    virtual void SetVisible( bool visible ) { if( group_ ) group_->setVisible( visible ); }
    virtual bool IsVisible() const { return ( group_ ) ? group_->isVisible() : false; }

    virtual bool HasParameter( const Param_ABC& param ) const;
    virtual void FixOrigin( bool fix ) const;
    void ActivateSwitchEditor( bool activated );
    virtual void SetSwitchEditorChecked( bool checked );
    //@}

    //! @name Accessors
    //@{
    void SetType( const std::string& type );
    const std::string& GetType() const;
    virtual int GetIndex( Param_ABC* ) const;
    virtual QString GetName() const;
    virtual const std::string& GetKeyName() const;
    QString GetMenuName() const;
    virtual bool IsChecked() const;
    virtual bool IsOptional() const;
    void SetEnabled( bool enabled );
    void SetOptional( bool optional );
    void ActivateOptionalIfNeeded( const actions::Parameter_ABC& param );
    virtual void SetParentList( ListParameterBase* parentList );
    virtual void SetName( const QString& name );
    virtual void SetKeyName( const std::string& keyName );
    //@}

public slots:
    //! @name Slots
    //@{
    virtual void OnMenuClick();
    void Update();
    //@}

protected:
    //! @name Types
    //@{
    typedef std::vector< QAction* > T_Actions;
    //@}

protected:
    //! @name Protected Helper
    //@{
    bool IsInList() const;
    bool IsInParam() const;
    QWidget* CreateSwitchEditor();
    virtual kernel::ContextMenu::T_MenuVariant CreateMenu( kernel::ContextMenu& menu );
    virtual void CreateInternalMenu( kernel::ContextMenu& menu );
    void CreateListMenu( QTreeView* list, const QStandardItemModel& model, const QPoint& pos, bool createEnabled ); // $$$$ ABR 2012-03-23: Here for translation ... enjoy Qt translation
    QString GetNextNameAndId( const QStandardItemModel& model ); // $$$$ NPT 2012-10-25: Here for translation ... enjoy Qt translation
    //@}

private:
    //! @name Helper
    //@{
    void ConnectAction();
    void ConnectWithParentList();
    //@}

protected:
    //! @name Member data
    //@{
    kernel::Controllers& controllers_;
    const ParamInterface_ABC& paramInterface_;
    ListParameterBase* parentList_;
    Param_ABC* parentParameter_;
    QString name_;
    std::string type_;
    ::gui::RichGroupBox* group_;
    kernel::OrderParameter parameter_;
    kernel::ContextMenu::T_MenuVariant internalMenu_;
    bool registered_;
    QAbstractButton* switchEditor_;
    bool needSwitchEditor_;
    //@}
};

    }
}

#endif // __Param_ABC_h_
