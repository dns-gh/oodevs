// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __BriefingDialog_h_
#define __BriefingDialog_h_

#include "gaming/CommandHandler_ABC.h"

namespace kernel
{
    class Controllers;
}

namespace tools
{
    class ExerciseConfig;
}

class CommandPublisher;
class CommandHandler;
class Publisher_ABC;
class Q3TextBrowser;

// =============================================================================
/** @class  BriefingDialog
    @brief  BriefingDialog
*/
// Created: SBO 2008-07-02
// =============================================================================
class BriefingDialog : public QDialog
                     , public CommandHandler_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             BriefingDialog( QWidget* parent, kernel::Controllers& controllers, Publisher_ABC& publisher, CommandHandler& handler, const tools::ExerciseConfig& config );
    virtual ~BriefingDialog();
    //@}

    //! @name Operations
    //@{
    virtual void Receive( const Command& command );
    //@}

private:
    //! @name Helpers
    //@{
    void Load( const tools::Path& filename );
    virtual void reject();
    virtual void accept();
    //@}

private:
    //! @name Member data
    //@{
    const tools::ExerciseConfig& config_;
    std::unique_ptr< CommandPublisher > publisher_;
    CommandHandler& handler_;
    Q3TextBrowser* browser_;
    std::string dialogId_;
    //@}
};

#endif // __BriefingDialog_h_
