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

class ReportsFactory : public boost::noncopyable
{
public:
    //! @name Constructors/Destructor
    //@{
             ReportsFactory( Publisher_ABC& publisher, AgentsModel& agents );
    virtual ~ReportsFactory();
    //@}

private:
    //! @name helper
    //@{
    void ClearReports();
    void SendRequest( unsigned int report = 0);
    void FillReports( const sword::ListReportsAck& ack );
    //@}

private:
    //! @name Member data
    //@{
    Publisher_ABC& publisher_;
    AgentsModel& agents_;
    int tick_;
    //@}
};

#endif // __ReportsFactory_h_
