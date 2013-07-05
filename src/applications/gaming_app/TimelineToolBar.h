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

namespace tools
{
    class ExerciseConfig;
}

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
             TimelineToolBar( QWidget* parent, const tools::ExerciseConfig& config, bool isMain );
    virtual ~TimelineToolBar();
    //@}

signals:
    //! @name Signals
    //@{
    void AddView();
    void RemoveCurrentView();

    void LoadOrderFileRequest( const tools::Path& path );
    void SaveOrderFileRequest( const tools::Path& path );
    //@}

public slots:
    //! @name Slots
    //@{
    void OnCenterView();
    void OnFilterSelection();
    void OnLoadOrderFile();
    void OnSaveOrderFile();
    //@}

private:
    //! @name Member data
    //@{
    const tools::ExerciseConfig& config_;
    //@}
};

#endif // __TimelineToolBar_h_
