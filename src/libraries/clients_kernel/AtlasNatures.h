// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __AtlasNatures_h_
#define __AtlasNatures_h_

#include <tools/Resolver.h>

namespace kernel
{
    class AtlasNature;

// =============================================================================
/** @class  AtlasNatures
    @brief  AtlasNatures
*/
// Created: SBO 2007-05-24
// =============================================================================
class AtlasNatures : public  tools::Resolver< AtlasNature >
{
public:
    //! @name Constructors/Destructor
    //@{
             AtlasNatures();
    virtual ~AtlasNatures();
    //@}

    //! @name Operations
    //@{
    AtlasNature MakeNature( unsigned short value ) const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    AtlasNatures( const AtlasNatures& );            //!< Copy constructor
    AtlasNatures& operator=( const AtlasNatures& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    void AddField( const QString& name, unsigned short value );
    //@}
};

}

#endif // __AtlasNatures_h_
