// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#ifndef __SerializationTools_h_
#define __SerializationTools_h_

// =============================================================================
// BOOST AUTO_PTR serialization
// =============================================================================

namespace boost {
    namespace serialization {

/////////////////////////////////////////////////////////////
// implement serialization for auto_ptr<T>
// note: this must be added to the boost namespace in order to
// be called by the library
template<class Archive, class T>
inline void save(
    Archive & ar,
    const std::auto_ptr<T> &t,
    const unsigned int /*file_version*/
){
    // only the raw pointer has to be saved
    // the ref count is rebuilt automatically on load
    T* tmp = t.get();
    ar << tmp;
}

template<class Archive, class T>
inline void load(
    Archive & ar,
    std::auto_ptr<T> &t,
    const unsigned int /*file_version*/
){
    T *pTarget;
    ar >> pTarget;
    t.reset(pTarget);
}

// split non-intrusive serialization function member into separate
// non intrusive save/load member functions
template<class Archive, class T>
inline void serialize(
    Archive & ar,
    std::auto_ptr<T> &t,
    const unsigned int file_version
){
    boost::serialization::split_free(ar, t, file_version);
}

template<class Archive, class T>
inline void save(
    Archive & ar,
    const boost::shared_ptr<T> &t,
    const unsigned int /*file_version*/
){
    // only the raw pointer has to be saved
    // the ref count is rebuilt automatically on load
    T* tmp = t.get();
    ar << tmp;
}

template<class Archive, class T>
inline void load(
    Archive & ar,
    boost::shared_ptr<T> &t,
    const unsigned int /*file_version*/
){
    T *pTarget;
    ar >> pTarget;
    t.reset(pTarget);
}

template<class Archive, class T>
inline void serialize(
    Archive & ar,
    boost::shared_ptr< T > & t,
    const unsigned int file_version ){
    boost::serialization::split_free(ar, t, file_version);
}

    } // end namespace serialization
} // end namespace boost

#endif // __SerializationTools_h_