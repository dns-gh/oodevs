// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __MissionInterface_ABC_h_
#define __MissionInterface_ABC_h_

#include "ParamInterface_ABC.h"
#include "actions/ActionsModel.h"
#include "clients_kernel/ModesObserver_ABC.h"
#include "clients_kernel/SafePointer.h"
#include "tools/Observer_ABC.h"
#include <tools/Path.h>

namespace kernel
{
    class Controllers;
    class Entity_ABC;
    class OrderType;
    class Time_ABC;
}

namespace gui
{
    class GlTools_ABC;
    class RichCheckBox;
    class RichDateTimeEdit;
    class Viewport_ABC;
}

namespace tools
{
    class ExerciseConfig;
    class Path;
}

namespace actions
{
    class Action_ABC;
    class ActionsModel;

    namespace gui
    {
        class Param_ABC;
        class InterfaceBuilder_ABC;

// =============================================================================
/** @class  MissionInterface_ABC
    @brief  MissionInterface_ABC
*/
// Created: APE 2004-04-20
// =============================================================================
class MissionInterface_ABC : public QTabWidget
                           , public ParamInterface_ABC
{
    Q_OBJECT

public:
    //! @name Constructors/Destructor
    //@{
             MissionInterface_ABC( QWidget* parent, const QString& name, kernel::Controllers& controllers,
                                   actions::ActionsModel& actionModel, const tools::ExerciseConfig& config );
    virtual ~MissionInterface_ABC();
    //@}

    //! @name Abstract method
    //@{
    //virtual void Publish() = 0;
    //@}

    //! @name Operations
    //@{
    bool IsEmpty() const;
    bool CheckValidity();
    void AddParameter( const QString& objectName, Param_ABC& parameter );
    void Draw( ::gui::GlTools_ABC& tools, ::gui::Viewport_ABC& extent ) const;
    void Fill( InterfaceBuilder_ABC& builder, const kernel::Entity_ABC& entity, const kernel::OrderType& orderType, const std::string& missionSheetPhysicalTag );
    void Purge();
    void SetPlanned( bool planned );
    void CommitTo( actions::Action_ABC& action ) const;
    //@}

    //! @name ParamInterface_ABC implementation
    //@{
    virtual QString Title() const;
    virtual int GetIndex( Param_ABC* param ) const;

    template< typename T >
    void Publish()
    {
        if( !order_ )
            return;
        Action_ABC* action = model_.CreateAction( *entity_, static_cast< const T& >( *order_ ) );
        CommitTo( *action );
        if( planned_ )
            emit PlannedMission( *action );
        else
            model_.Publish( *action, 0 );
    }
    //@}

signals:
    //! @name Signals
    //@{
    void PlannedMission( const actions::Action_ABC& );
    //@}

protected:
    //! @name Types
    //@{
    typedef std::vector< Param_ABC* >      T_Parameters; // $$$$ ABR 2012-01-11: Todo : use sharepointer
    typedef T_Parameters::const_iterator CIT_Parameters;
    //@}

protected:
    //! @name Member data
    //@{
    kernel::Controllers& controllers_;
    actions::ActionsModel& model_;
    const tools::ExerciseConfig& config_;

    kernel::SafePointer< kernel::Entity_ABC > entity_;
    const kernel::OrderType* order_;
    T_Parameters parameters_;

    QString title_;
    QWidget* mainTab_;
    QWidget* optionalTab_;
    QWidget* helpTab_;
    std::vector< QWidget* > widgetToDelete_;
    bool planned_;
    //@}
};

    }
}


#endif // __MissionInterface_ABC_h_
