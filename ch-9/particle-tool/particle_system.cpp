#include "game.hpp"

void ParticleSystem::AddEmitter( Emitter* emitter ) {
    m_EmitterList.push_back( emitter );
}

void ParticleSystem::Start() {
    m_Active = true;
}

void ParticleSystem::Stop() {
    m_Active = false;
}

void ParticleSystem::Move() {
    Emitter* emitter;
    std::vector<Emitter*>::iterator it;

    for( it = m_EmitterList.begin(); it != m_EmitterList.end(); it++ ) {
        emitter = *it;
        emitter->Move();
    }

    m_Active = false;
}

/*
class ParticleSystem {
    std::vector<Emitter*> m_EmitterList;

    ParticleSystem();
    void Start();
    void Stop();
    void Move();

    void AddEmitter( Emitter* emitter );
};
*/