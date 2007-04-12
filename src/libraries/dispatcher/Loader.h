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

class InputBinaryWrapper;

namespace dispatcher
{
    class SimulationDispatcher;

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
             Loader( SimulationDispatcher& simulation, const std::string& directory );
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
    void LoadOutSimMessage( std::ifstream& input, unsigned count );
    void LoadOutSimMessage( InputBinaryWrapper& input );
    void LoadInClientMessage( InputBinaryWrapper& input );
    //@}

private:
    //! @name Member data
    //@{
    SimulationDispatcher& simulation_;
    T_Frames frames_;
    T_KeyFrames keyFrames_;
    std::ifstream updates_;
    std::ifstream keys_;
    unsigned currentFrame_;

    ASN1OCTET buffer_[10000];
    //@}
};

}

#endif // __Loader_h_
