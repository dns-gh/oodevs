// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __GradientPreferences_h_
#define __GradientPreferences_h_

namespace kernel
{
    class Options;
}

namespace gui
{
    class Gradient;

// =============================================================================
/** @class  GradientPreferences
    @brief  GradientPreferences
*/
// Created: SBO 2007-07-03
// =============================================================================
class GradientPreferences
{
public:
    //! @name Types
    //@{
    typedef std::shared_ptr< Gradient > T_Gradient;
    //@}

public:
    //! @name Constructors/Destructor
    //@{
             GradientPreferences();
    explicit GradientPreferences( const GradientPreferences& other );
    virtual ~GradientPreferences();
    //@}

    //! @name Operations
    //@{
    void Load( const kernel::Options& options );

    void Add( const T_Gradient& gradient );
    void Remove( const T_Gradient& gradient );
    size_t Count() const;

    void Apply( const std::function< void( const T_Gradient ) >& functor ) const;

    T_Gradient GetByName( const QString& name ) const;
    T_Gradient GetByDisplayName( const QString& name ) const;
    T_Gradient GetCurrent() const;
    void SetCurrent( const T_Gradient& );
    //@}

private:
    //! @name Member data
    //@{
    std::vector< T_Gradient > gradients_;
    T_Gradient current_;
    //@}
};

} //! namespace gui

#endif // __GradientPreferences_h_
