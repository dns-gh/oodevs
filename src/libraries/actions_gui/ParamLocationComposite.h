// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#ifndef __ParamLocationComposite_h_
#define __ParamLocationComposite_h_

#include "Param_ABC.h"
#include "clients_kernel/OrderParameter.h"

class QStackedWidget;

namespace kernel
{
    class CoordinateConverter_ABC;
}

namespace actions
{
    namespace gui
    {
        class InterfaceBuilder_ABC;
        class ListParameterBase;

// =============================================================================
/** @class  ParamLocationComposite
    @brief  ParamLocationComposite
*/
// Created: LDC 2010-08-18
// =============================================================================
class ParamLocationComposite: public Param_ABC // $$$$ ABR 2013-06-14: Should be rename in ParamComposite, it can handle any type of Param_ABC
{
    Q_OBJECT

public:
    //! @name Constructors/Destructor
    //@{
             ParamLocationComposite( const InterfaceBuilder_ABC& builder, const kernel::OrderParameter& parameter );
    virtual ~ParamLocationComposite();
    //@}

    //! @name Operations
    //@{
    virtual void CreateInternalMenu( kernel::ContextMenu& mainMenu );
    Param_ABC* AddElement( const std::string& type, const std::string& name = "", bool forceNotOptional = false );
    void Purge();
    virtual int GetIndex( Param_ABC* param ) const;
    virtual void SetParentList( ListParameterBase* parentList );
    virtual void SetName( const QString& name );
    virtual void RegisterIn();
    virtual void RemoveFromController();

    virtual void Draw( const geometry::Point2f& point, const ::gui::Viewport_ABC& viewport, ::gui::GLView_ABC& tools ) const;
    virtual QWidget* BuildInterface( const QString& objectName, QWidget* parent );
    virtual void CommitTo( actions::ParameterContainer_ABC& container ) const;
    virtual bool CheckValidity() const;
    virtual void SetEntity( const kernel::Entity_ABC* entity );
    virtual bool HasParameter( const Param_ABC& param ) const;

    virtual void Visit( const actions::parameters::Agent& param );
    virtual void Visit( const actions::parameters::Automat& param );
    virtual void Visit( const actions::parameters::Location& param );
    virtual void Visit( const actions::parameters::ObjectKnowledge& param );
    virtual void Visit( const actions::parameters::Path& param );
    virtual void Visit( const actions::parameters::Point& param );
    virtual void Visit( const actions::parameters::Polygon& param );
    virtual void Visit( const actions::parameters::PopulationKnowledge& param );
    virtual void Visit( const actions::parameters::UrbanBlock& param );
    //@}

protected:
    //! @name Helpers
    //@{
    template< typename T >
    void InternalVisit( const T& param, const std::string& type );
    void InternalBuildInterface();
    virtual void NotifyChanged( Param_ABC& param );
    virtual bool IsChecked() const;
    virtual void SetSwitchEditorChecked( bool checked );
    //@}

private:
    //! @name Types
    //@{
    typedef std::vector<Param_ABC*>    T_Params;
    typedef T_Params::iterator        IT_Params;
    typedef T_Params::const_iterator CIT_Params;
    //@}

private slots:
    //! @name Slots
    //@{
    void OnChecked( bool checked );
    void OnSwitchEditorClicked( bool checked );
    //@}

protected:
    //! @name Member data
    //@{
    const InterfaceBuilder_ABC& builder_;
    std::vector< Param_ABC* > params_;
    std::vector< QWidget* > widgets_;
    QStackedWidget* stack_;
    Param_ABC* selectedParam_;
    //@}
};

    }
}

#endif // __ParamLocationComposite_h_
