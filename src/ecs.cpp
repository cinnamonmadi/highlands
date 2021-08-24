#include "ecs.hpp"

// ECS

ECS::ECS() {
    for(Entity entity = 0; entity < MAX_ENTITIES; entity++) {
        available_entities.push(entity);
    }
}

Entity ECS::create_entity() {
    assert(entity_count < MAX_ENTITIES);

    Entity new_id = available_entities.front();
    available_entities.pop();
    entity_count++;

    return new_id;
}

void ECS::destroy_entity(Entity entity) {
    assert(entity < MAX_ENTITIES);

    // reset entity signature
    available_entities.push(entity);
    entity_count--;

    // notify each component array of the entity deletion
    for(uint8_t i = 0; i < component_count; i++) {
        component_arrays[i]->handle_entity_destroyed(entity);
    }
}

void ECS::set_signature(Entity entity, Signature signature) {
    assert(entity < MAX_ENTITIES);

    entity_signatures[entity] = signature;
}

Signature ECS::get_signature(Entity entity) {
    assert(entity < MAX_ENTITIES);

    return entity_signatures[entity];
}
