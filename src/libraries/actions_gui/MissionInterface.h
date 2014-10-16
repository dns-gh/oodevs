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
#include "clients_kernel/SafePointer.h"

namespace kernel
{
    class Controllers;
    class Entity_ABC;
    class OrderType;
}

namespace tools
{
    class ExerciseConfig;
}

namespace actions
{
    namespace gui
    {

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
    void AddParameter( const QString& objectName, Param_ABC& parameter );
    //@}

    //! @name MissionInterface_ABC implementation
    //@{
    virtual bool IsFilling() const;
    virtual bool HasParameter( const Param_ABC& parameter ) const;
    virtual void Draw( ::gui::GLView_ABC& tools, ::gui::Viewport_ABC& extent ) const;
    virtual bool CheckValidity();
    virtual void Build( InterfaceBuilder_ABC& builder, const kernel::OrderType& orderType, E_MissionType type );
    virtual void Rebuild( InterfaceBuilder_ABC& builder );
    virtual void Purge();
    virtual void SetEntity( const kernel::Entity_ABC* entity );
    virtual void CommitTo( actions::Action_ABC& action ) const;
    virtual void FillFrom( const actions::Action_ABC& action );
    virtual void FixOrigin( bool fix ) const;
    //@}

    //! @name ParamInterface_ABC implementation
    //@{
    virtual QString Title() const;
    virtual int GetIndex( Param_ABC* param ) const;
    //@}

private:
    //! @name Types
    //@{
    typedef std::vector< Param_ABC* >      T_Parameters; // $$$$ ABR 2012-01-11: Todo : use sharepointer
    typedef T_Parameters::const_iterator CIT_Parameters;
    //@}

    //! @name Helpers
    //@{
    void PurgeParameters();
    //@}

private:
    //! @name Member data
    //@{
    const tools::ExerciseConfig& config_;

    kernel::SafePointer< kernel::Entity_ABC > entity_;
    const kernel::OrderType* order_;
    T_Parameters parameters_;

    QWidget* mainTab_;
    QWidget* optionalTab_;
    QWidget* helpTab_;
    std::vector< QWidget* > widgetToDelete_;
    E_MissionType type_;

    static std::map< E_MissionType, const std::string > missionSheetPhysicalTags_;
    bool filling_;
    //@}
};

    }
}


#endif // __MissionInterface_h_
