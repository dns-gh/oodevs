// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef __FilterDialog_h_
#define __FilterDialog_h_

#include <boost/noncopyable.hpp>

namespace tools
{
    class ExerciseConfig;
}

namespace xml
{
    class xistream;
}

class Filter_ABC;
class FilterManager;
class Menu;
class QSize;

// =============================================================================
/** @class  FilterDialog
    @brief  FilterDialog
*/
// Created: ABR 2011-06-20
// =============================================================================
class FilterDialog : public QDialog
                   , private boost::noncopyable
{
    Q_OBJECT

public:
    //! @name Constructors/Destructor
    //@{
             FilterDialog( QWidget* parent, xml::xistream& xis, const tools::ExerciseConfig& config );
    virtual ~FilterDialog();
    //@}

    //! @name Operations
    //@{
    void Load();
    void AddFilter( Filter_ABC& filter );
    void InsertMenuEntry( Menu& menu );
    //@}

private:
    //! @name Helpers
    //@{
    virtual QSize sizeHint() const;
    virtual void closeEvent( QCloseEvent * e );
    //@}

private slots:
    //! @name Slots
    //@{
    void OnSelectFilter( int index );
    void OnAccept();
    void OnReject();
    //@}

signals:
    //! @name Signals
    //@{
    void reloadExercise();
    //@}

private:
    //! @name Member data
    //@{
    std::auto_ptr< FilterManager > filterManager_;
    Q3ListBox*                     list_;
    QLabel*                        description_;
    QPushButton*                   okButton_;
    Q3WidgetStack*                 stack_;
    //@}

public:
    //! @name Public static member data
    //@{
    static const int               menuIndex_;
    //@}
};

#endif // __FilterDialog_h_
