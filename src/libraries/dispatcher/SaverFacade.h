// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __SaverFacade_h_
#define __SaverFacade_h_

#include "game_asn/Asn.h"
#include "MessageFilter.h"

namespace dispatcher
{
    class Model;
    class Saver;

// =============================================================================
/** @class  SaverFacade
    @brief  SaverFacade
*/
// Created: AGE 2007-04-10
// =============================================================================
class SaverFacade
{

public:
    //! @name Constructors/Destructor
    //@{
             SaverFacade( Model& model, const std::string& directory );
    virtual ~SaverFacade();
    //@}

    //! @name Operations
    //@{
    void Save( ASN1T_MsgsOutSim& asnMsg );
    //@}

private:
    //! @name Copy/Assignment
    //@{
    SaverFacade( const SaverFacade& );            //!< Copy constructor
    SaverFacade& operator=( const SaverFacade& ); //!< Assignement operator
    //@}

    //! @name Helpers
    //@{
    void SaveUpdate( ASN1T_MsgsOutSim& asnMsg );
    void StartFrame( ASN1T_MsgsOutSim& asnMsg );
    void EndFrame( ASN1T_MsgsOutSim& asnMsg );
    //@}

private:
    //! @name Member data
    //@{
    Model& model_;
    std::auto_ptr< Saver > saver_;
    ASN1OCTET aASNEncodeBuffer_[100000];
    unsigned frameCount_;

    MessageFilter filter_;
    //@}
};

}

#endif // __SaverFacade_h_
