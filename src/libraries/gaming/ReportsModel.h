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
}

class ReportsModel : public boost::noncopyable
{
public:
    //! @name Constructors/Destructor
    //@{
             ReportsModel( Publisher_ABC& publisher, AgentsModel& agents,
                           const kernel::Profile_ABC& profile );
    virtual ~ReportsModel();
    //@}

public:
    //! @name Types
    //@{
    typedef std::vector< sword::Report > T_Reports;
    typedef std::vector< sword::Trace >  T_Traces;
    //@}

    //! @name Accessors
    //@{
    const T_Reports& GetReports( unsigned int entity ) const;
    const T_Traces& GetTraces( unsigned int entity ) const;
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
    void AddReport( const sword::Report& report );
    //@}

private:
    //! @name Member data
    //@{
    Publisher_ABC& publisher_;
    AgentsModel& agents_;
    const kernel::Profile_ABC& profile_;
    int tick_;
    int context_;
    std::map< unsigned int, T_Reports > reports_;
    std::map< unsigned int, T_Traces > traces_;
    // Entity with unread reports
    std::deque< unsigned int > entities_;
    //@}
};

#endif // __ReportsModel_h_
