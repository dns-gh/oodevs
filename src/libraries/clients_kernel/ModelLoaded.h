// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ModelLoaded_h_
#define __ModelLoaded_h_

namespace kernel
{

// =============================================================================
/** @class  ModelLoaded
    @brief  Model loaded "event"
*/
// Created: SBO 2006-05-24
// =============================================================================
class ModelLoaded
{

public:
    //! @name Constructors/Destructor
    //@{
    explicit ModelLoaded( const std::string& scipioXml );
    virtual ~ModelLoaded();
    //@}

    //! @name Member data
    //@{
    std::string scipioXml_;
    //@}

private:
    //! @name Copy/Assignement
    //@{
    ModelLoaded( const ModelLoaded& );            //!< Copy constructor
    ModelLoaded& operator=( const ModelLoaded& ); //!< Assignement operator
    //@}
};

class ModelUnLoaded
{
public:
             ModelUnLoaded() {}
    virtual ~ModelUnLoaded() {}
};

}

#endif // __ModelLoaded_h_
