// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2013 MASA Group
//
// *****************************************************************************

#ifndef __TimelineFilteredViewWidget_h_
#define __TimelineFilteredViewWidget_h_

namespace timeline
{
    struct Configuration;
    struct Error;
    struct Event;
    class Server_ABC;
}

class Config;
class Model;
class Simulation;

// =============================================================================
/** @class  TimelineFilteredViewWidget
    @brief  TimelineFilteredViewWidget
*/
// Created: ABR 2013-05-28
// =============================================================================
class TimelineFilteredViewWidget : public QWidget
{
    Q_OBJECT

public:
    //! @name Constructors/Destructor
    //@{
             TimelineFilteredViewWidget( QWidget* parent, const Simulation& simulation, Model& model, timeline::Configuration& cfg, int viewNumber, const QStringList& filters );
    virtual ~TimelineFilteredViewWidget();
    //@}

    //! @name Operations
    //@{
    void Connect();
    void Disconnect();

    int GetViewNumber() const;
    //@}

private:
    //! @name Helpers
    //@{
    void CreateDummyMission(); // $$$$ ABR 2013-05-24: Test method
    //@}

signals:
    //! @name Signals
    //@{
    void CreateEventSignal( const timeline::Event& event );
    void DeleteEventSignal( const std::string& uuid );

    void AddNewFilteredView( const QStringList& filters );
    void RemoveCurrentFilteredView();
    //@}

private slots:
    //! @name Slots
    //@{
    void CreateEvent( const timeline::Event& event );
    void DeleteEvent( const std::string& uuid );

    void OnCreatedEvent( const timeline::Event& event, const timeline::Error& error );
    void OnDeletedEvent( const std::string& uuid, const timeline::Error& error );

    void OnSelectedEvent( boost::shared_ptr< timeline::Event > event );
    void OnActivatedEvent( const timeline::Event& event );
    void OnContextMenuEvent( boost::shared_ptr< timeline::Event > event );
    void OnKeyUp( int key );

    void OnFilterSelectionChanged( const QStringList& );
    //@}

private:
    //! @name Member data
    //@{
    int viewNumber_;
    QToolBar* toolBar_;
    QWidget* timelineWidget_;
    QVBoxLayout* mainLayout_;
    const Simulation& simulation_;
    Model& model_;

    std::auto_ptr< timeline::Server_ABC > server_;
    std::auto_ptr< timeline::Configuration > cfg_;
    boost::shared_ptr< timeline::Event > selected_;

    std::vector< std::string > creationRequestedEvents_;
    std::vector< std::string > deletionRequestedEvents_;
    //@}
};

#endif // __TimelineFilteredViewWidget_h_
