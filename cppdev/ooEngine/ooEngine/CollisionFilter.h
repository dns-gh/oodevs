#ifndef _COLLISION_FILTER_H_
#define _COLLISION_FILTER_H_

enum CollisionCategory
{
    Default  = 0x00,
    Groupe1  = 0x01,
    Groupe2  = 0x02,
    Groupe3  = 0x04,
    Groupe4  = 0x08,
    Groupe5  = 0x16,
    Groupe6  = 0x32,
    Groupe7  = 0x64,
};

class CollisionFilter
{
public:
    CollisionFilter();
    CollisionFilter( unsigned int category, unsigned int mask, unsigned int groupeIndex ); 
    void UpdateCategory( unsigned int );
    void UpdateCollisionMask( unsigned int );
    unsigned int GetCategory() const;
    unsigned int GetCollisionMask() const;
    unsigned int GetGroupIndex() const;

private:
    unsigned int category_;
    unsigned int collisionMask_;
    unsigned int groupIndex_; // 0 means no collisions at all
};

#endif // CollisionFilter.h