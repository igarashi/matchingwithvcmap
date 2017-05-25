//
// Created by igarashi on 5/24/2017.
//

#ifndef PVXMATCHING_INJECTIVE_MAP_HPP
#define PVXMATCHING_INJECTIVE_MAP_HPP

#include <map>
#include <utility>

namespace utils {
    namespace injective_map {
        
        enum InsertionResult {
            Inserted = 0,
            AlreadyExists = 1,
            KeyDuplicated = 2,
            ValueDuplicated = 3
        };
        
        class InjectiveMap {
        public:
            
            std::pair<std::map<int, int>::iterator, injective_map::InsertionResult>
            insert(const int &key, const int &value) {
                auto key_exists = map_.insert({key, value});
                if (!key_exists.second) {
                    if (key_exists.first->second == value)
                        return std::make_pair(key_exists.first, AlreadyExists);
                    else
                        return std::make_pair(key_exists.first, KeyDuplicated);
                }
                
                auto value_exists = inversed_map_.insert({value, key});
                if (!value_exists.second) {
                    // remove item
                    map_.erase(key_exists.first);
                    return std::make_pair(map_.end(), ValueDuplicated);
                }
                
                return std::make_pair(key_exists.first, Inserted);
            };
            
            void clear() {
                map_.clear();
                inversed_map_.clear();
            }
            
            const int operator[](const int &key) const {
                auto value = map_.find(key);
                if (value != map_.end())
                    return value->second;
                
                throw "Invalid Key";
            }
            
            std::map<int, int>::const_iterator begin() const {
                return map_.begin();
            }
            
            std::map<int, int>::const_iterator end() const {
                return map_.end();
            }
            
            InjectiveMap() {};
            
            InjectiveMap(const InjectiveMap &data) : map_(data.map_), inversed_map_(data.inversed_map_) {};
        
        private:
            std::map<int, int> map_;
            std::map<int, int> inversed_map_;
        };
    }
}

#endif //PVXMATCHING_INJECTIVE_MAP_HPP
