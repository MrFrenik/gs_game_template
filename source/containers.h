#ifndef CONTAINERS_H
#define CONTAINERS_H 

/*
 * Simple cpp template wrappers for gs_containers
 */

/*
// Hash table
template <typename K, typename V>
struct hash_table_t 
{ 
    hash_table_t() {
        data = nullptr;
    }

    ~hash_table_t() {
        gs_hash_table_free(data);
    }

    gs_hash_table(K, V) data;

    void insert(K key, V value) {
        gs_hash_table_insert(data, key, value);
    }

    bool exists(const K& key) {
	return data ? gs_hash_table_key_exists(data, key) : false;
    }

    V* getp(K key) {
        return gs_hash_table_getp(data, key);
    }

    V get(K key) {
        return gs_hash_table_get(data, key);
    }

    uint32_t iter_new() {
    	return gs_hash_table_iter_new(data);
    }

    bool iter_valid(uint32_t it) {
        return gs_hash_table_iter_valid(data, it);
    }

    void iter_advance(uint32_t& it) {
        gs_hash_table_iter_advance(data, it);
    }
}; 

// Dynamic Array
template <typename T>
struct dyn_array_t 
{ 
    dyn_array_t() {
        data = nullptr;
    }

    ~dyn_array_t() {
        gs_dyn_array_free(data);
    }

    void push(const T& val) {
        gs_dyn_array_push(data, val);
    }

    uint32_t size() {
        return gs_dyn_array_size(data);
    }

    uint32_t capacity() {
        return gs_dyn_array_capacity(data);
    }

    T pop() {
        T v = gs_dyn_array_back(data);
        gs_dyn_array_pop(data);
        return v;
    }

    T operator[](const uint32_t& idx) {
        return data[idx];
    } 

    gs_dyn_array(T) data;
}; 

// Slot array
template <typename T>
struct slot_array_t 
{
    slot_array_t() {
        data = nullptr;
    }

    ~slot_array_t() {
        gs_slot_array_free(data);
    }

    uint32_t insert(const T& v) {
        return gs_slot_array_insert(data, v);
    }

    T get(const uint32_t& hndl) {
        return gs_slot_array_get(data, hndl);
    }

    T* getp(const uint32_t& hndl) {
        return gs_slot_array_getp(data, hndl);
    }

    uint32_t iter_new() {
        return gs_slot_array_iter_new(data);
    }

    bool iter_valid(uint32_t it) {
        return gs_slot_array_iter_valid(data, it);
    }

    void iter_advance(uint32_t& it) {
        gs_slot_array_iter_advance(data, it);
    }

    gs_slot_array(T) data;
};
*/

#endif
