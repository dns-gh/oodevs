// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __PreferencesList_h_
#define __PreferencesList_h_

namespace gui
{

// =============================================================================
/** @class  PreferencesList
    @brief  PreferencesList
*/
// Created: SBO 2007-01-03
// =============================================================================
class PreferencesList : public QListView
{
public:
    //! @name Constructors/Destructor
    //@{
             PreferencesList( QWidget* parent, QWidgetStack& pages );
    virtual ~PreferencesList();
    //@}

    //! @name Operations
    //@{
    void AddPage( const QString& name, QWidget* widget );
    virtual void setCurrentItem( QListViewItem* item );
    //@}

private:
    //! @name Copy/Assignement
    //@{
    PreferencesList( const PreferencesList& );            //!< Copy constructor
    PreferencesList& operator=( const PreferencesList& ); //!< Assignement operator
    //@}

    //! @name Types
    //@{
    typedef std::map< QString, QWidget* > T_Widgets;
    //@}

private:
    //! @name Member data
    //@{
    QWidgetStack& pages_;
    T_Widgets widgets_;
    //@}
};

}

#endif // __PreferencesList_h_
