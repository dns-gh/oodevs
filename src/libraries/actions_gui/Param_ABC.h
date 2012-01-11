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

#include "tools/Observer_ABC.h"
#include "clients_kernel/ContextMenu.h"
#include "clients_kernel/Drawable_ABC.h"
#include "clients_kernel/OrderParameter.h"
#include "clients_gui/RichGroupBox.h"
#include <boost/noncopyable.hpp>
#include "tools/Resolver.h"

namespace kernel
{
    class ActionController;
    class GlTools_ABC;
    class Viewport_ABC;
    class AgentKnowledgeConverter_ABC;
    class CoordinateConverter_ABC;
    class OrderParameter;
    class Entity_ABC;
    class Controller;
    class ObjectTypes;
    class Object_ABC;
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
    class ParameterContainer_ABC;

    namespace gui
    {
        class ListParameterBase;

// =============================================================================
/** @class  Param_ABC
    @brief  Param_ABC
*/
// Created: APE 2004-04-20
// =============================================================================
class Param_ABC : public QObject
                , public tools::Observer_ABC
                , public kernel::Drawable_ABC
                , private boost::noncopyable
{
    Q_OBJECT

public:
    //! @name Constructors/Destructor
    //@{
             Param_ABC( QObject* parent, const ParamInterface_ABC& paramInterface, const kernel::OrderParameter& parameter );
    virtual ~Param_ABC();
    //@}

public:
    //! @name Operations
    //@{
    void RegisterListener( Param_ABC& param );
    void NotifyChange();
    virtual void NotifyChanged( Param_ABC& param );
    virtual void RemoveFromController();
    virtual void RegisterIn( kernel::ActionController& controller );
    virtual void Draw( const geometry::Point2f& point, const kernel::Viewport_ABC& viewport, const kernel::GlTools_ABC& tools ) const;
    virtual QWidget* BuildInterface( QWidget* parent );
    virtual bool CheckValidity();
    virtual bool InternalCheckValidity() const;
    virtual void CommitTo( actions::ParameterContainer_ABC& ) const = 0;
    //@}

    //! @name Accessors
    //@{
    void SetType( const std::string& type );
    const std::string& GetType() const;
    virtual int GetIndex( Param_ABC* ) const;
    virtual QString GetName() const;
    QString GetMenuName() const;
    virtual bool IsChecked() const;
    virtual bool IsOptional() const;
    void SetOptional( bool optional );
    virtual void SetParentList( ListParameterBase* parentList );
    virtual void SetName( const QString& name );
    //@}

public slots:
    //! @name Slots
    //@{
    virtual void OnMenuClick();
    //@}

protected:
    //! @name Types
    //@{
    typedef std::vector< QAction* >     T_Actions;
    typedef T_Actions::const_iterator CIT_Actions;
    //@}

protected:
    //! @name Protected Helper
    //@{
    bool IsInList() const;
    bool IsInParam() const;
    virtual kernel::ContextMenu::T_MenuVariant CreateMenu( kernel::ContextMenu& menu );
    virtual void CreateInternalMenu( kernel::ContextMenu& menu );
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
    const ParamInterface_ABC&           paramInterface_;
    ListParameterBase*                  parentList_;
    Param_ABC*                          parentParameter_;
    QString                             name_;
    std::string                         type_;
    ::gui::RichGroupBox*                group_;
    kernel::OrderParameter              parameter_;
    kernel::ActionController*           controller_;
    kernel::ContextMenu::T_MenuVariant  internalMenu_;
    //@}
};

    }
}

#endif // __Param_ABC_h_
