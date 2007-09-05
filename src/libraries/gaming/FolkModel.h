// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __FolkModel_h_
#define __FolkModel_h_

#include "game_asn/asn.h"
#include <boost/multi_array.hpp>

namespace kernel
{
    class Controller;
}

// =============================================================================
/** @class  FolkModel
    @brief  FolkModel
*/
// Created: AGE 2007-09-04
// =============================================================================
class FolkModel
{
public:
    //! @name Types
    //@{
    typedef std::vector< QString > T_Names;
    //@}

public:
    //! @name Constructors/Destructor
    //@{
    explicit FolkModel( kernel::Controller& controller );
    virtual ~FolkModel();
    //@}

    //! @name Operations
    //@{
    void Update( const ASN1T_MsgFolkCreation& creation );
    void Update( const ASN1T_MsgFolkGraphUpdate& update );

    // $$$$ AGE 2007-09-05: ...
    const T_Names& Containers() const;
    const T_Names& Profiles() const;
    const T_Names& Activities() const;
    void SwitchTo( const QString& container, const QString& profile, const QString& activity );

    const std::vector< float >& GetCurrentsRatios();
    //@}

private:
    //! @name Copy/Assignment
    //@{
    FolkModel( const FolkModel& );            //!< Copy constructor
    FolkModel& operator=( const FolkModel& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    void Update( const ASN1T_MsgFolkGraphEdgeUpdate& update );
    void ComputeRatios(); 
    void NormalizeRatios();
    //@}

    //! @name Types
    //@{
    typedef boost::multi_array< int, 4 > T_Values; // données par edge / container / profile / activity
    typedef std::vector< float >         T_Ratios;
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controller& controller_;

    T_Names containers_;
    T_Names profiles_;
    T_Names activities_;
    unsigned edgeCount_;
    unsigned edgeSize_;

    std::auto_ptr< T_Values > values_;   

    int currentContainer_, currentProfile_, currentActivity_;
    T_Ratios ratios_;
    bool dirty_;
    //@}
};

#endif // __FolkModel_h_
