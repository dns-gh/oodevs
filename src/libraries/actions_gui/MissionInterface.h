// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __MissionInterface_h_
#define __MissionInterface_h_

#include "ParamInterface_ABC.h"
#include "MissionInterface_ABC.h"
#include "actions/ActionsModel.h"
#include "clients_kernel/SafePointer.h"


#include "clients_kernel/FragOrderType.h"
#include "clients_kernel/MissionType.h"

namespace kernel
{
    class Controllers;
    class Entity_ABC;
    class OrderType;
}

namespace gui
{
    class GlTools_ABC;
    class Viewport_ABC;
}

namespace timeline
{
    struct Event;
}

namespace tools
{
    class ExerciseConfig;
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
/** @class  MissionInterface
    @brief  MissionInterface
*/
// Created: APE 2004-04-20
// =============================================================================
class MissionInterface : public QTabWidget
                       , public ParamInterface_ABC
                       , public MissionInterface_ABC
{
    Q_OBJECT

public:
    //! @name Constructors/Destructor
    //@{
             MissionInterface( QWidget* parent, const QString& name, kernel::Controllers& controllers, const tools::ExerciseConfig& config );
    virtual ~MissionInterface();
    //@}

    //! @name Operations
    //@{
    bool IsEmpty() const;
    bool CheckValidity();
    void AddParameter( const QString& objectName, Param_ABC& parameter );
    void Draw( ::gui::GlTools_ABC& tools, ::gui::Viewport_ABC& extent ) const;
    void Rebuild( InterfaceBuilder_ABC& builder );
    virtual void Build( InterfaceBuilder_ABC& builder, const kernel::OrderType& orderType, E_MissionType type );
    virtual void SetEntity( const kernel::Entity_ABC* entity );
    virtual void Purge();
    virtual void SetPlanned( bool planned );
    void CommitTo( actions::Action_ABC& action ) const;
    virtual void FillFrom( const actions::Action_ABC& action );
    virtual void PublishFragOrder( actions::ActionsModel& model, timeline::Event* event = 0 ) const
    {
        Publish< kernel::FragOrderType >( model, event );
    }
    virtual void PublishMissionOrder( actions::ActionsModel& model, timeline::Event* event = 0 ) const
    {
        Publish< kernel::MissionType >( model, event );
    }
    //@}

    //! @name ParamInterface_ABC implementation
    //@{
    virtual QString Title() const;
    virtual int GetIndex( Param_ABC* param ) const;
    //@}

signals:
    //! @name Signals
    //@{
    void PlannedMission( const actions::Action_ABC&, timeline::Event* ) const;
    //@}

private:
    //! @name Types
    //@{
    typedef std::vector< Param_ABC* >      T_Parameters; // $$$$ ABR 2012-01-11: Todo : use sharepointer
    typedef T_Parameters::const_iterator CIT_Parameters;
    //@}

    //! @name Helpers
    //@{
    template< typename T >
    void Publish( actions::ActionsModel& model, timeline::Event* event = 0 ) const
    {
        if( !order_ )
            return;
        Action_ABC* action = model.CreateAction( static_cast< const T& >( *order_ ), entity_ );
        CommitTo( *action );
        if( planned_ )
            emit PlannedMission( *action, event );
        else
            model.Publish( *action, 0 );
    }
    void PurgeParameters();
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controllers& controllers_;
    const tools::ExerciseConfig& config_;

    kernel::SafePointer< kernel::Entity_ABC > entity_;
    const kernel::OrderType* order_;
    T_Parameters parameters_;

    QWidget* mainTab_;
    QWidget* optionalTab_;
    QWidget* helpTab_;
    std::vector< QWidget* > widgetToDelete_;
    bool planned_;
    E_MissionType type_;

    static std::map< E_MissionType, const std::string > missionSheetPhysicalTags_;
    //@}
};

    }
}


#endif // __MissionInterface_h_
