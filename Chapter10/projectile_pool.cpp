#include "game.hpp"

ProjectilePool::ProjectilePool() {
    for( int i = 0; i < 10; i++ ) {
        m_ProjectileList.push_back( new Projectile() );
    }
}

ProjectilePool::~ProjectilePool() {
    m_ProjectileList.clear();
}

void ProjectilePool::MoveProjectiles() {
    Projectile* projectile;
    std::vector<Projectile*>::iterator it;

    for( it = m_ProjectileList.begin(); it != m_ProjectileList.end(); it++ ) {
        projectile = *it;
        if( projectile->m_Active ) {
            projectile->Move();
        }
    }

}

void ProjectilePool::RenderProjectiles() {
    Projectile* projectile;
    std::vector<Projectile*>::iterator it;

    for( it = m_ProjectileList.begin(); it != m_ProjectileList.end(); it++ ) {
        projectile = *it;
        if( projectile->m_Active ) {
            projectile->Render();
        }
    }
}

Projectile* ProjectilePool::GetFreeProjectile() {
    Projectile* projectile;
    std::vector<Projectile*>::iterator it;
    for( it = m_ProjectileList.begin(); it != m_ProjectileList.end(); it++ ) {
        projectile = *it;
        if( projectile->m_Active == false ) {
            return projectile;
        }
    }
    return NULL;
}
