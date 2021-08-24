#pragma once

#include <cstdint>
#include <bitset>
#include <queue>
#include <memory>
#include <cassert>
#include <cstring>

using Entity = uint32_t;
const uint32_t MAX_ENTITIES = 5000;

using ComponentType = uint8_t;
const uint8_t MAX_COMPONENTS = 32;
const uint8_t COMPONENT_NOT_FOUND = -1;

using Signature = std::bitset<MAX_COMPONENTS>;

class IComponentArray;

template<typename T>
class ComponentArray;

class ECS {
    public:
        ECS();

        Entity create_entity();
        void destroy_entity(Entity entity);
        void set_signature(Entity entity, Signature signature);
        Signature get_signature(Entity entity);

        template<typename T>
        void register_component() {
            assert(get_component_index<T>() == COMPONENT_NOT_FOUND);

            const char* type_name = typeid(T).name();
            component_types[component_count] = type_name;
            component_arrays[component_count] = std::make_shared<ComponentArray<T>>();
            component_count++;
        }

        template<typename T>
        uint8_t get_component_index() {
            const char* type_name = typeid(T).name();

            for(uint8_t i = 0; i < component_count; i++) {
                if(strcmp(component_types[i], type_name) == 0) {
                    return i;
                }
            }

            return COMPONENT_NOT_FOUND;
        }

        template<typename T>
        void add_component(Entity entity, T component) {
            get_component_array<T>()->add_component(entity, component);
        }

        template<typename T>
        void remove_component(Entity entity) {
            get_component_array<T>()->remove_component(entity);
        }

        template<typename T>
        T& get_component(Entity entity) {
            return get_component_array<T>()->get_component(entity);
        }
    private:
        std::queue<Entity> available_entities;
        Signature entity_signatures[MAX_ENTITIES];
        uint32_t entity_count;

        const char* component_types[MAX_COMPONENTS];
        std::shared_ptr<IComponentArray> component_arrays[MAX_COMPONENTS];
        uint8_t component_count;

        template<typename T>
        std::shared_ptr<ComponentArray<T>> get_component_array() {
            uint8_t component_index = get_component_index<T>();

            assert(component_index != COMPONENT_NOT_FOUND);

            return std::static_pointer_cast<ComponentArray<T>>(component_arrays[component_index]);
        }
};

class IComponentArray {
    public:
        virtual ~IComponentArray() = default;
        virtual void handle_entity_destroyed(Entity entity) = 0;
};

template<typename T>
class ComponentArray : public IComponentArray {
    public:
        ComponentArray() {
            size = 0;
        }

        void add_component(Entity entity, T component) {
            assert(!entity_exists(entity));

            entity_to_index[entity] = size;
            index_to_entity[size] = entity;
            elements[size] = component;
            size++;
        }

        void remove_component(Entity entity) {
            assert(entity_exists(entity));

            size_t removed_index = entity_to_index[entity];
            elements[removed_index] = elements[size - 1];

            Entity last_entity = index_to_entity[size - 1];
            entity_to_index[last_entity] = removed_index;
            index_to_entity[removed_index] = last_entity;

            size--;
        }

        T& get_component(Entity entity) {
            assert(entity_exists(entity));

            return elements[entity_to_index[entity]];
        }

        void handle_entity_destroyed(Entity entity) override {
            if(entity_exists(entity)) {
                remove_component(entity);
            }
        }
    private:
        T elements[MAX_ENTITIES];
        size_t entity_to_index[MAX_ENTITIES];
        Entity index_to_entity[MAX_ENTITIES];
        size_t size;

        bool entity_exists(Entity entity) {
            for(size_t i = 0; i < size; i++) {
                if(index_to_entity[i] == entity) {
                    return true;
                }
            }

            return false;
        }
};
