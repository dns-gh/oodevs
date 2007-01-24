// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __DEC_PathFactory_h_
#define __DEC_PathFactory_h_

#include "MIL.h"

// =============================================================================
// Created: SBO 2006-03-27
// =============================================================================
class DEC_PathFactory
{
public:
    //! @name Singleton
    //@{
    static void  Initialize( MIL_InputArchive& archive );
    static void  Terminate ();
    static const DEC_PathFactory& GetInstance();
    //@}

public:
    //! @name Constructors/Destructor
    //@{
    explicit DEC_PathFactory( MIL_InputArchive& archive );
    virtual ~DEC_PathFactory();
    //@}

private:
    //! @name Copy/Assignement
    //@{
    DEC_PathFactory( const DEC_PathFactory& );            //!< Copy constructor
    DEC_PathFactory& operator=( const DEC_PathFactory& ); //!< Assignement operator
    //@}

private:
    //! @name Singleton
    //@{
    static DEC_PathFactory* pInstance_;
    //@}
};

#endif // __DEC_PathFactory_h_
