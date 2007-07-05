// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __Loader_h_
#define __Loader_h_

#include "game_asn/Asn.h"
#include "Frames.h"
#include <vector>

namespace tools
{
    class InputBinaryWrapper;
}

namespace dispatcher
{
    class Config;
    class ReplayModel_ABC;

// =============================================================================
/** @class  Loader
    @brief  Loader
*/
// Created: AGE 2007-04-10
// =============================================================================
class Loader
{

public:
    //! @name Constructors/Destructor
    //@{
             Loader( ReplayModel_ABC& model, const Config& config, const std::string& records );
    virtual ~Loader();
    //@}

    //! @name Operations
    //@{
    void SkipToFrame( unsigned count ); 
    bool Tick();

    unsigned GetCurrentTick() const;
    unsigned GetTickNumber() const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    Loader( const Loader& );            //!< Copy constructor
    Loader& operator=( const Loader& ); //!< Assignement operator
    //@}

    //! @name Types
    //@{
    typedef std::vector< Frame >    T_Frames;
    typedef std::vector< KeyFrame > T_KeyFrames;
    //@}

    //! @name Helpers
    //@{
    void LoadIndex( const std::string& file );
    void LoadKeyIndex( const std::string& file );
    void LoadSimToClientMessage( std::ifstream& input, unsigned count );
    void LoadSimToClientMessage( tools::InputBinaryWrapper& input );
    void LoadKeyFrame( unsigned frame );
    bool RequiresKeyFrame( unsigned frame );
    //@}

private:
    //! @name Member data
    //@{
    ReplayModel_ABC& model_;
    T_Frames frames_;
    T_KeyFrames keyFrames_;
    std::ifstream updates_;
    std::ifstream keys_;
    unsigned currentFrame_;

    ASN1OCTET buffer_[100000];
    //@}
};

}

#endif // __Loader_h_
