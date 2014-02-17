// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __TesselatorBase_h_
#define __TesselatorBase_h_

#include "Tesselator_ABC.h"

class GLUtesselator;

// =============================================================================
/** @class  TesselatorBase
    @brief  %TesselatorBase
*/
// Created: AGE 2005-01-10
// =============================================================================
class TesselatorBase : public Tesselator_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
    explicit TesselatorBase( unsigned int windingRule );
    virtual ~TesselatorBase();
    //@}

    //! @name Operations
    //@{
    virtual void StartPath();
    virtual void AddPoints( const T_Points& points );
    virtual void AddPoint( const geometry::Point& point );
    virtual void EndPath( bool close = true );

    void Commit();
    //@}

    //! @name Callbacks
    //@{
    void Combine( const geometry::Point& point, unsigned int& index );
    void Vertex( unsigned int index );
    //@}

    //! @name Operations
    //@{
    virtual void Begin( unsigned int type ) = 0;
    virtual void Vertex( const geometry::Point& point, unsigned int index ) = 0;
    virtual void End() = 0;
    //@}

protected:
    //! @name Helpers
    //@{
    const T_Points& GetPoints() const;
    //@}

private:
    //! @name Copy/Assignement
    //@{
    TesselatorBase( const TesselatorBase& );            //!< Copy constructor
    TesselatorBase& operator=( const TesselatorBase& ); //!< Assignement operator
    //@}

private:
    //! @name Member data
    //@{
    GLUtesselator* pTesselatorBase_;
    T_Points points_;
    bool started_;
    //@}
};

#endif // __TesselatorBase_h_
