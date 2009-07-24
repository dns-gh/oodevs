// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ReportManager_ABC_h_
#define __ReportManager_ABC_h_

#include <boost/shared_ptr.hpp>

namespace xml
{
  class xostream;
}

namespace plugins
{
namespace xmlia
{

// =============================================================================
/** @class  ReportManager_ABC
@brief  ReportManager interface
*/
// Created: SLG 2009-06-12
// =============================================================================
class ReportManager_ABC
{

public:
  //! @name Constructors/Destructor
  //@{
  ReportManager_ABC() {};
  virtual ~ReportManager_ABC() {};
  //@}

  //! @name Operations
  //@{
  virtual void PushReports() = 0;
  virtual void ReceiveReports() = 0;
  //@}
};

}
}

#endif // __ReportManager_ABC_h_
