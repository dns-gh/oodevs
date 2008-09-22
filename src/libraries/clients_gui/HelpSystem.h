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
    Q_OBJECT;
public:
    //! @name Constructors/Destructor
    //@{
             HelpSystem( QWidget* root, const std::string& config );
    virtual ~HelpSystem();
    //@}

public slots:
    //! @name Slots
    //@{
    void ShowHelp();
    //@}

private:
    //! @name Copy/Assignment
    //@{
    HelpSystem( const HelpSystem& );            //!< Copy constructor
    HelpSystem& operator=( const HelpSystem& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    void ReadWidget( xml::xistream& xis );
    std::string FindWidget( const QWidget* root );
    //@}

    //! @name Types
    //@{
    typedef std::map< std::string, std::string >  T_Anchors;
    typedef T_Anchors::const_iterator           CIT_Anchors;
    //@}

private:
    //! @name Member data
    //@{
    QWidget* root_;
    T_Anchors anchors_;
    //@}
};

}

#endif // __HelpSystem_h_
