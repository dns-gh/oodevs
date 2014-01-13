// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __HelpSystem_h_
#define __HelpSystem_h_

namespace kernel
{
    class ModeController;
    class Profile_ABC;
}

namespace xml
{
    class xistream;
}

namespace gui
{

// =============================================================================
/** @class  HelpSystem
    @brief  Help system
*/
// Created: AGE 2008-08-18
// =============================================================================
class HelpSystem : public QObject
{
    Q_OBJECT
public:
    //! @name Constructors/Destructor
    //@{
             HelpSystem( QWidget* root,
                         const tools::Path& config,
                         const kernel::ModeController* modeController = 0,
                         const kernel::Profile_ABC* profile = 0 );
    virtual ~HelpSystem();
    //@}

public slots:
    //! @name Slots
    //@{
    void ShowHelp();
    //@}

private:
    //! @name Helpers
    //@{
    void RetrieveHelpFile();
    void ReadWidget( xml::xistream& xis );
    std::string FindWidget( const QObject* root );
    //@}

    //! @name Types
    //@{
    typedef std::map< std::string, std::string >  T_Anchors;
    typedef T_Anchors::const_iterator           CIT_Anchors;
    //@}

private:
    //! @name Member data
    //@{
    const kernel::ModeController* modeController_;
    const kernel::Profile_ABC* profile_;
    tools::Path helpFile_;
    QWidget* root_; // à virer
    T_Anchors anchors_;
    //@}
};

}

#endif // __HelpSystem_h_
