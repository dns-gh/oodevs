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

QT_FORWARD_DECLARE_CLASS( QTextEdit )

#include "ParamInterface_ABC.h"
#include <boost/noncopyable.hpp>
#pragma warning( push, 0 )
#include <Qt3Support/q3vbox.h>
#pragma warning( pop )
#include "clients_kernel/ModesObserver_ABC.h"
#include "tools/Observer_ABC.h"

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

namespace tools
{
    class ExerciseConfig;
}

class QTabWidget;
class QPushButton;

namespace actions
{
    class Action_ABC;

    namespace gui
    {
        class Param_ABC;

// =============================================================================
/** @class  MissionInterface_ABC
    @brief  MissionInterface_ABC
*/
// Created: APE 2004-04-20
// =============================================================================
class MissionInterface_ABC : public Q3VBox
                           , public ParamInterface_ABC
                           , public tools::Observer_ABC
                           , public kernel::ModesObserver_ABC
{
    Q_OBJECT

public:
    //! @name Constructors/Destructor
    //@{
             MissionInterface_ABC( QWidget* parent, const kernel::OrderType& order, kernel::Entity_ABC& entity, kernel::Controllers& controllers, const tools::ExerciseConfig& config, std::string missionSheetPath = "" );
    virtual ~MissionInterface_ABC();
    //@}

    //! @name Operations
    //@{
    void Draw( ::gui::GlTools_ABC& tools, ::gui::Viewport_ABC& extent ) const;
    bool IsEmpty() const;
    void AddParameter(  const QString& objectName, Param_ABC& parameter );
    //@}

    //! @name ParamInterface_ABC implementation
    //@{
    virtual QString Title() const;
    virtual int GetIndex( Param_ABC* param ) const;
    //@}

public slots:
    //! @name Slots
    //@{
    virtual void OnOk();
    //@}

signals:
    //! @name Signals
    //@{
    void OkClicked();
    //@}

protected:
    //! @name Helpers
    //@{
    virtual void NotifyModeChanged( E_Modes newMode );
    const kernel::Entity_ABC& GetEntity() const;
    void CreateTitle( const QString& title );
    void CommitTo( actions::Action_ABC& action ) const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    virtual void paintEvent( QPaintEvent*, QString title );
    //@}

    //! @name Helpers
    //@{
    bool CheckValidity();
    void Commit();
    virtual void Publish() = 0;
    //@}

    //! @name Types
    //@{
    typedef std::vector< Param_ABC* >      T_Parameters; // $$$$ ABR 2012-01-11: Todo : use sharepointer
    typedef T_Parameters::const_iterator CIT_Parameters;
    //@}

private:
    //! @name Member data
    //@{
    QString title_;
    kernel::Controllers& controllers_;
    kernel::Entity_ABC& entity_;
    T_Parameters parameters_;
    QTabWidget* tabs_;
    Q3VBox* mainTab_;
    Q3VBox* optionalTab_;
    Q3ScrollView* sc1_;
    QPushButton* ok_;
    //@}
};

    }
}

#endif // __MissionInterface_ABC_h_
