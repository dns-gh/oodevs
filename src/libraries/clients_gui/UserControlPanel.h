// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __UserControlPanel_h_
#define __UserControlPanel_h_

#include "clients_kernel/OptionsObserver_ABC.h"
#include "PreferencePanel_ABC.h"

namespace kernel
{
    class Controllers;
    class Options;
}

namespace gui
{
    class ButtonGroup;

// =============================================================================
/** @class  UserControlPanel
    @brief  UserControlPanel
*/
// Created: RPD 2008-08-12
// =============================================================================
class UserControlPanel : public PreferencePanel_ABC
                   , public kernel::Observer_ABC
                   , public kernel::OptionsObserver_ABC
{
    Q_OBJECT;

public:
    //! @name Constructors/Destructor
    //@{
             UserControlPanel( QWidget* pParent, kernel::Controllers& controllers );
    virtual ~UserControlPanel();
    //@}

    //! @name Operations
    //@{
    virtual void Commit();
    virtual void Reset();
    //@}

private slots:
    //! @name Helpers
    //@{
    void OnMapDraggingType( int type );
    //@}

private:
    //! @name Copy / Assignment
    //@{
    UserControlPanel( const UserControlPanel& );
    UserControlPanel& operator=( const UserControlPanel& );
    //@}

    //! @name Helpers
    //@{
    virtual void OptionChanged( const std::string& name, const kernel::OptionVariant& value );
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controllers& controllers_;
    kernel::Options&     options_;
    ButtonGroup* mapDraggingType_;
    //@}
};

}

#endif // __UserControlPanel_h_
