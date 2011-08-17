// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __TER_StaticData_h_
#define __TER_StaticData_h_

class StaticTerrainGraph;

// =============================================================================
/** @class  TER_StaticData
    @brief  %TER_StaticData
*/
// Created: CMA 2011-08-16
// =============================================================================
class TER_StaticData
{
public:
    //! @name Constructors/Destructor
    //@{
     TER_StaticData( const std::string& strGraphArchive, const std::string& strNodeArchive, const std::string& strLinkArchive, float precision );
    ~TER_StaticData();
    //@}

    //! @name Cast to member
    //@{
    operator const StaticTerrainGraph&() const { return *pGraph_; }
    //@}

private:
    //! @name Copy/Assignemet
    //@{
    TER_StaticData( const TER_StaticData& );            //!< Copy constructor
    TER_StaticData& operator=( const TER_StaticData& ); //!< Assignment operator
    //@}

private:
    //! @name Member data
    //@{
    StaticTerrainGraph* pGraph_;
    //@}
};

#endif // __TER_StaticData_h_
