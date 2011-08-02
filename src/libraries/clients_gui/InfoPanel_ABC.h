// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __InfoPanel_ABC_h_
#define __InfoPanel_ABC_h_

namespace gui
{
    class PanelStack_ABC;

// =============================================================================
/** @class  InfoPanel_ABC
    @brief  InfoPanel_ABC
*/
// Created: AGE 2005-04-05
// =============================================================================
class InfoPanel_ABC : public QScrollArea
{
public:
    //! @name Constructors/Destructor
    //@{
             InfoPanel_ABC( QWidget* parent, PanelStack_ABC& panel, const QString& name, const char* qtName = 0 );
    virtual ~InfoPanel_ABC();
    //@}

    //! @name Operations
    //@{
    void Show();
    void Hide();
    bool IsVisible() const;
    //@}

    //! @name Accessors
    //@{
    const QString& GetName() const { return name_; }
    //@}

private:
    //! @name Copy/Assignment
    //@{
    InfoPanel_ABC( const InfoPanel_ABC& );            //!< Copy constructor
    InfoPanel_ABC& operator=( const InfoPanel_ABC& ); //!< Assignment operator
    //@}

protected:
    //! @name Member data
    //@{
    PanelStack_ABC& stack_;
    QString name_;
    //@}
};

}

#endif // __InfoPanel_ABC_h_
