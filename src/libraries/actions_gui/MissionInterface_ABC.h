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

#include "ParamComboBox.h"

#pragma warning( push, 0 )
#include <Qt3Support/q3vbox.h>
#pragma warning( pop )

namespace kernel
{
    class ActionController;
    class Entity_ABC;
    class OrderType;
}

class Q3ScrollView;
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
{
    Q_OBJECT;

public:
    //! @name Constructors/Destructor
    //@{
             MissionInterface_ABC( QWidget* parent, const kernel::OrderType& order, kernel::Entity_ABC& entity, kernel::ActionController& controller );
    virtual ~MissionInterface_ABC();
    //@}

    //! @name Operations
    //@{
    QString Title() const;
    void Draw( const kernel::GlTools_ABC& tools, kernel::Viewport_ABC& extent ) const;
    bool IsEmpty() const;
    void AddParameter( Param_ABC& parameter );
    void ChangeOkValueButton( bool planningMode );
    //@}

public slots:
    //! @name Slots
    //@{
    virtual void OnOk();
    //@}

signals:
    void OkClicked();

protected:
    //! @name Helpers
    //@{
    const kernel::Entity_ABC& GetEntity() const;
    void CreateTitle( const QString& title );
    void CreateOkCancelButtons();
    void CommitTo( actions::Action_ABC& action ) const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    virtual void paintEvent( QPaintEvent*, QString title );
    MissionInterface_ABC( const MissionInterface_ABC& );
    MissionInterface_ABC& operator=( const MissionInterface_ABC& );
    //@}

    //! @name Helpers
    //@{
    bool CheckValidity();
    void Commit();
    virtual void Publish() = 0;
    //@}

    //! @name Types
    //@{
    typedef std::vector< Param_ABC* >      T_Parameters;
    typedef T_Parameters::const_iterator CIT_Parameters;
    //@}

private:
    //! @name Member data
    //@{
    QString title_;
    kernel::ActionController& controller_;
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
