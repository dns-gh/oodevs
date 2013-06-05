// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2013 MASA Group
//
// *****************************************************************************

#ifndef __TimelineToolBar_h_
#define __TimelineToolBar_h_

// =============================================================================
/** @class  TimelineToolBar
    @brief  TimelineToolBar
*/
// Created: ABR 2013-05-28
// =============================================================================
class TimelineToolBar : public QToolBar
{
    Q_OBJECT

public:
    //! @name Constructors/Destructor
    //@{
             TimelineToolBar( QWidget* parent, bool isMain, const QStringList& activeFilters );
    virtual ~TimelineToolBar();
    //@}

    //! @name Operations
    //@{
    //@}

signals:
    //! @name Signals
    //@{
    void FilterSelectionChanged( const QStringList& filters );
    void AddNewFilteredView( const QStringList& filters );
    void RemoveCurrentFilteredView();
    //@}

public slots:
    //! @name Slots
    //@{
    void OnCenterView();
    void OnFilterSelection();
    void OnAddNewFilteredView();
    void OnRemoveCurrentFilteredView();
    //@}

private:
    //! @name Helpers
    //@{
    void AddFilter( const QString& filter, const QString& displayName, const QStringList& filters );
    void AddFilter( const QString& filter, const QString& displayName, bool isActive );

    QStringList GetActiveFilters();
    //@}

    //! @name Types
    //@{
    struct Filter
    {
        QString filter_;
        QString displayName_;
        bool isActive_;
    };
    //@}


private:
    //! @name Member data
    //@{
    std::vector< Filter > filters_;
    QMenu* filtersMenu_;
    //@}
};

#endif // __TimelineToolBar_h_
