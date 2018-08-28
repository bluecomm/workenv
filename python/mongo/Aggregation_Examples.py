from pymongo import MongoClient
from bson.son import SON
import pprint

def print_sep_line(comment, total_len=80, sep='*'):
    char_len = (total_len - len(comment)) / 2 - 1
    sep_str = sep * char_len + ' ' + comment + ' ' + sep * char_len
    if len(sep_str) != total_len:
        sep_str += sep
    print sep_str

######################## Setup
print_sep_line("Setup")
db = MongoClient().aggregation_example
result = db.things.insert_many([
    {"x": 1, "tags": ["dog", "cat"]},
    {"x": 2, "tags": ["cat"]},
    {"x": 2, "tags": ["mouse", "cat", "dog"]},
    {"x": 3, "tags": []}])
print result.inserted_ids

######################## Aggregation Framework
print_sep_line("Aggregation Framework")
pipeline = [
    {"$unwind": "$tags"},
    {"$group": {"_id": "$tags", "count": {"$sum": 1}}},
    {"$sort": SON([("count", -1), ("_id", -1)])}
]
pprint.pprint(list(db.things.aggregate(pipeline)))

# To run an explain plan for this aggregation use the command() method:
db.command('aggregate', 'things', pipeline=pipeline, explain=True)

######################## Map/Reduce
print_sep_line("Map/Reduce")
from bson.code import Code
mapper = Code("""
              function () {
                this.tags.forEach(function(z) {
                  emit(z, 1);
                });
              }
              """)

reducer = Code("""
               function (key, values) {
                 var total = 0;
                 for (var i = 0; i < values.length; i++) {
                   total += values[i];
                 }
                 return total;
               }
               """)

result = db.things.map_reduce(mapper, reducer, "myresults")
for doc in result.find():
    pprint.pprint(doc)

######################## Advanced Map/Reduce
print_sep_line("Advanced Map/Reduce")
pprint.pprint(db.things.map_reduce(mapper, reducer, "myresults", full_response=True))
print_sep_line("full_response", 60, '-')

results = db.things.map_reduce(mapper, reducer, "myresults", query={"x": {"$lt": 2}})
for doc in results.find():
    pprint.pprint(doc)
print_sep_line("query", 60, '-')

pprint.pprint(
    db.things.map_reduce(
        mapper,
        reducer,
        out=SON([("replace", "results"), ("db", "outdb")]),
        full_response=True))
