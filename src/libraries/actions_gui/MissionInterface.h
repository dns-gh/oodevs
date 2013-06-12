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
#include "actions/ActionsModel.h"
#include "clients_kernel/SafePointer.h"

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
    void Fill( InterfaceBuilder_ABC& builder, const kernel::OrderType& orderType, E_MissionType type );
    void SetEntity( const kernel::Entity_ABC* entity );
    void Purge();
    void SetPlanned( bool planned );
    void CommitTo( actions::Action_ABC& action ) const;
    template< typename T, typename Creator >
    void Publish( Creator& creator, timeline::Event* event = 0 ) const
    {
        if( !order_ )
            return;
        Action_ABC* action = creator.CreateAction( *entity_, static_cast< const T& >( *order_ ) );
        CommitTo( *action );
        if( planned_ )
            emit PlannedMission( *action, event );
        else
            creator.Publish( *action, 0 );
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

    static std::map< E_MissionType, const std::string > missionSheetPhysicalTags_;
    //@}
};

    }
}


#endif // __MissionInterface_h_
