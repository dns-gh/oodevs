// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __Composer_h_
#define __Composer_h_

#include "Reductor_ABC.h"

#pragma warning( push )
#pragma warning( disable : 4355 )

// =============================================================================
/** @class  Composer
    @brief  Composer
    // $$$$ AGE 2007-10-08: ^c^v KeyMarshaller
*/
// Created: AGE 2007-10-08
// =============================================================================
template< typename K, typename A1, typename A2 >
class Composer : public Slot_ABC
{
public:
    //! @name Types
    //@{
    typedef K Key_Type;
    typedef A2 Result_Type;
    //@}

public:
    //! @name Constructors/Destructor
    //@{
    explicit Composer( Function1_ABC< K, A2 >& next )
        : function_( next )
        , first_   ( this )
        , second_  ( this )
    {
        // NOTHING
    }
    virtual ~Composer()
    {
        // NOTHING
    }
    //@}

    //! @name Operations
    //@{
    Function1_ABC< A1, A2 >& F() { return second_; }
    Function1_ABC< K,  A1 >& G() { return first_; }
    //@}

private:
    //! @name Operations
    //@{
    virtual std::string GetName() const { return "Composer"; }
    virtual Slot_ABC* GetSubSlot( unsigned slot )
    {
        return slot == 0 ? &second_:
               slot == 1 ? &first_ :
               static_cast< Slot_ABC* >( 0 );
    }
    //@}

    //! @name Helpers
    //@{
    template< typename K, typename T >
    struct Function : public Reductor_ABC< K, T >
    {
        Function( Composer* that )
            : that_   ( that )
            , done_   ( false )
            , hasKey_ ( false )
            , current_()
        {
            // NOTHING
        }
        virtual void OnBeginTick() {}
        virtual void SetKey( const K& key )
        {
            hasKey_ = true;
            current_ = key;
        }
        virtual void Apply( const T& arg )
        {
            values_[ current_ ] = arg;
        }
        virtual void OnEndTick()
        {
            done_ = true;
            that_->Commit();
        }
        void Reset()
        {
            done_ = false;
            hasKey_ = false;
            current_ = K();
            values_.clear();
        }
        Composer* that_;
        bool done_;
        bool hasKey_;
        K current_;
        std::map< K, T > values_;
    };

    void Commit()
    {
        if( first_.done_ && second_.done_ )
        {
            function_.BeginTick();
            if( first_.hasKey_ && second_.hasKey_ )
                MergeKeys();
            else if( first_.hasKey_ )
                ForwardFirst();
            else if( second_.hasKey_ )
                ForwardSecond();
            else
                function_.Apply( second_.values_[ K() ] );
            first_.Reset();
            second_.Reset();
            function_.EndTick();
        }
    }
    void MergeKeys()
    {
        std::map< K, A1 >::const_iterator firstIt  = first_.values_.begin();
        std::map< K, A1 >::const_iterator endIt = first_.values_.end();
        for( ; firstIt != endIt; ++firstIt )
        {
            std::map< A1, A2 >::const_iterator secondIt = second_.values_.find( firstIt->second );
            if( secondIt != second_.values_.end() )
            {
                function_.SetKey( firstIt->first );
                function_.Apply ( secondIt->second );
            }
        }
    }
    void ForwardFirst()
    {
        const A2& second = second_.values_[ K() ];
        for( std::map< K, A1 >::const_iterator it = first_.values_.begin(); it != first_.values_.end(); ++it )
        {
            function_.SetKey( it->first );
            function_.Apply( second );
        }
    }
    void ForwardSecond()
    {
        const A1& first = first_.values_[ K() ];
        std::map< A1, A2 >::const_iterator secondIt = second_.values_.find( first );
        if( secondIt != second_.values_.end() )
            function_.Apply ( secondIt->second );
    }
    //@}

private:
    //! @name Member data
    //@{
    Function1_ABC< K, A2 >& function_;
    Function< K,  A1 > first_;
    Function< A1, A2 > second_;
    //@}
};

#pragma warning( pop )

#endif // __Composer_h_
