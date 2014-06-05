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

namespace tools
{
    class ExerciseConfig;
}

namespace xml
{
    class xistream;
}

namespace gui
{
    class RichPushButton;
    template< typename T > class RichWidget;
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
{
    Q_OBJECT

public:
    //! @name Constructors/Destructor
    //@{
             FilterDialog( const QString& objectName, QWidget* parent, const QString& name, const tools::ExerciseConfig& config );
    virtual ~FilterDialog();
    //@}

    //! @name Operations
    //@{
    void Load( xml::xistream& xis );
    void AddFilter( Filter_ABC& filter );
    void InsertMenuEntry( Menu& menu );
    const QString GetName() const;
    const QKeySequence GetKeySequence() const;
    //@}

private:
    //! @name Helpers
    //@{
    virtual void closeEvent( QCloseEvent * e );
    virtual void showEvent ( QShowEvent * e );
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
    std::unique_ptr< FilterManager > filterManager_;
    gui::RichWidget< QListWidget >*           list_;
    QLabel*                        description_;
    gui::RichPushButton*           okButton_;
    QStackedWidget*                stack_;
    //@}
};

#endif // __FilterDialog_h_
