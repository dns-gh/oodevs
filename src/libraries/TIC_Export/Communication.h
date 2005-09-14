// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: SBO 2005-05-18 $
// $Archive: /MVW_v10/Build/SDK/TIC/src/Export/Communication.h $
// $Author: Sbo $
// $Modtime: 20/07/05 18:31 $
// $Revision: 7 $
// $Workfile: Communication.h $
//
// *****************************************************************************

#ifndef __TICExport_Communication_h_
#define __TICExport_Communication_h_

#include "TIC_Export.h"
#include <string>

namespace TIC
{
    class Communication_ABC;
}

namespace TICExport
{

// =============================================================================
/** @class  Communication
    @brief  Communication
    @par    Using example
    @code
    Communication;
    @endcode
*/
// Created: SBO 2005-05-25
// =============================================================================
class ENGINE_API Communication
{
    friend class CommunicationLink;

public:
	//! @name Constructors/Destructor
    //@{
             Communication();
    virtual ~Communication();
    //@}

    //! @name Accessors
    //@{
          unsigned int GetArity  () const;
    const std::string& GetType   () const;
    const std::string& GetSubType() const;
    const std::string& GetMessage() const;
          bool         IsReport  () const;
    //@}

private:
    //! @name Internal use constructor
    //@{
    Communication( const TIC::Communication_ABC& com );
    //@}

	//! @name Member data
    //@{
    const TIC::Communication_ABC* pCom_;
	//@}
};

} // end namespace TICExport

#endif // __TICExport_Communication_h_
