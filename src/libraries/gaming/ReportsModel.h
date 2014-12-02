// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2014 MASA Group
//
// *****************************************************************************

#ifndef __ReportsFactory_h_
#define __ReportsFactory_h_

class AgentsModel;
class Publisher_ABC;

namespace kernel
{
    class Profile_ABC;
    class Time_ABC;
}

class ReportsModel : public boost::noncopyable
{
public:
    //! @name Constructors/Destructor
    //@{
             ReportsModel( Publisher_ABC& publisher, AgentsModel& agents,
                           const kernel::Profile_ABC& profile, const kernel::Time_ABC& time );
    virtual ~ReportsModel();
    //@}

public:
    //! @name Types
    //@{
    struct Message
    {
        Message( unsigned int entity, const sword::Report& report,
                 const QDateTime& date )
            : entity_( entity )
            , report_( report )
            , date_  ( date )
        {}
        Message( unsigned int entity, const sword::Trace& trace,
            const QDateTime& date )
            : entity_( entity )
            , trace_ ( trace )
            , date_  ( date )
        {}
        unsigned int entity_;
        QDateTime date_;
        sword::Report report_;
        sword::Trace trace_;
    };
    typedef std::vector< Message > T_Messages;
    //@}

    //! @name Accessors
    //@{
    const T_Messages& GetReports( unsigned int entity ) const;
    bool HaveUnreadReports( unsigned int entity ) const;
    size_t UnreadReports() const;
    unsigned int NextUnreadReports() const;
    //@}

    //! @name Operations
    //@{
    void Purge();
    void Clear( unsigned int entity );
    void ClearTraces( unsigned int entity );
    void UpdateUnreadReports();
    void ReadReports();
    void ReadReports( unsigned int entity );
    //@}

private:
    //! @name Helpers
    //@{
    void SendRequest( int context, unsigned int report = 0 );
    void FillReports( const sword::ListReportsAck& ack );
    void AddMessage( const Message& message );
    void AddUnreadReports( unsigned int entity );
    //@}

private:
    //! @name Member data
    //@{
    Publisher_ABC& publisher_;
    AgentsModel& agents_;
    const kernel::Profile_ABC& profile_;
    const kernel::Time_ABC& time_;
    int tick_;
    int context_;
    std::map< unsigned int, T_Messages > messages_;
    // Entity with unread reports
    std::deque< unsigned int > entities_;
    //@}
};

#endif // __ReportsModel_h_
