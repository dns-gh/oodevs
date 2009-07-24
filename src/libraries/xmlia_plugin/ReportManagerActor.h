// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ReportManagerActor_h_
#define __ReportManagerActor_h_

#include <boost/thread/recursive_mutex.hpp>

namespace tools
{
  namespace thread
  {
    class ThreadPool;
  }
}

namespace plugins
{
namespace xmlia
{
  class Publisher_ABC;
  class ReportManager;

// =============================================================================
/** @class  ReportManagerActor
@brief  ReportManagerActor
*/
// Created: AGE 2008-05-30
// =============================================================================
class ReportManagerActor 
{

public:
  //! @name Constructors/Destructor
  //@{
  explicit ReportManagerActor( ReportManager& base );
  virtual ~ReportManagerActor();
  //@}

  //! @name Operations
  //@{
  virtual void Send( Publisher_ABC& publisher );
  virtual void Receive( Publisher_ABC& publisher );
  //@}

private:
  //! @name Copy/Assignment
  //@{
  ReportManagerActor( const ReportManagerActor& );            //!< Copy constructor
  ReportManagerActor& operator=( const ReportManagerActor& ); //!< Assignment operator
  //@}

  //! @name Helpers
  //@{
  void DoPushReports( Publisher_ABC& publisher );
  void DoReceiveReports( Publisher_ABC& publisher );
  //@}


private:
  //! @name Member data
  //@{
  ReportManager& base_;
  std::auto_ptr< tools::thread::ThreadPool > thread_;
  //@}
};

}
}

#endif // __PublisherActor_h_
