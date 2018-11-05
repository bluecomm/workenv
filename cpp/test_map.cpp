#include <iostream>
#include <map>
#include <string>
#include <stdint.h>

typedef struct AggStru_ {
    uint32_t key1;
    uint32_t key2;
    std::string key3;
    uint32_t value1;
    uint32_t value2;

    // AggStru_(uint32_t key1_, uint32_t key2_, std::string key3_, uint32_t value1_, uint32_t value2_) :
    //    key1(key1_), key2(key2_), key3(key3_), value1(value1_), value2(value2_) {}
} AggStru;

typedef AggStru_ AggStru;

typedef struct AggStru_Index_ {
    uint32_t key1;
    uint32_t key2;
    std::string key3;

    bool operator < (const AggStru_Index_ & cmp) const {
        if (key1 < cmp.key1) {
            return true;
        }
        if (key2 < cmp.key2) {
            return true;
        }
        if (key3.compare(cmp.key3) < 0) {
            return true;
        }
    }
} AggStru_Index;

typedef struct AggStru_Values_ {
    uint32_t value1;
    uint32_t value2;
} AggStru_Values;

typedef std::map<AggStru_Index, AggStru_Values> AggMap;

void aggregate_value(AggStru_Values* base, AggStru_Values* delta) {
    base->value1 += delta->value1;
    base->value2 += delta->value2;
}

void aggregate(AggMap& agg_map, AggStru& agg_stru) {

    AggStru_Index agg_indx;
    agg_indx.key1 = agg_stru.key1;
    agg_indx.key2 = agg_stru.key2;
    agg_indx.key3 = agg_stru.key3;

    AggStru_Values agg_values;
    agg_values.value1 = agg_stru.value1;
    agg_values.value2 = agg_stru.value2;

    std::map<AggStru_Index, AggStru_Values>::iterator iter = agg_map.find(agg_indx);
    if (iter == agg_map.end()) {
        agg_map[agg_indx] = agg_values;
    } else {
        aggregate_value(&(iter->second), &agg_values);
    }
    
}
int main () {
    AggMap agg_map;
    AggStru as1;
    as1.key1 = 11;
    as1.key2 = 12;
    as1.key3 = "13";
    as1.value1 = 11;
    as1.value2 = 12;

    AggStru as2;
    as2.key1 = 21;
    as2.key2 = 22;
    as2.key3 = "23";
    as2.value1 = 21;
    as2.value2 = 22;

    AggStru as3;
    as3.key1 = 11;
    as3.key2 = 12;
    as3.key3 = "13";
    as3.value1 = 31;
    as3.value2 = 32;

    aggregate(agg_map, as1);
    aggregate(agg_map, as2);
    aggregate(agg_map, as3);

    for (std::map<AggStru_Index, AggStru_Values>::iterator iter = agg_map.begin();
            iter != agg_map.end(); ++iter) {
        std::cout << iter->first.key1 << "\t" << iter->first.key2 << "\t" << iter->first.key3 << "\t";
        std::cout << iter->second.value1 << "\t" << iter->second.value2 << std::endl;
    }

    return 0;
}
