#!/usr/bin/python

dates = [
    "20171120",
    "20171121",
    "20171122",
    "20171123",
    "20171124",
]

souces = [
    {
        "source_name":"FCAspNormal",
        "source_path":["/app/ecom/FCAspNormal"] * len(dates),
        "source_date": dates,
    },
    {
        "source_name":"FCCharge",
        "source_path":["/app/ecom/FCCharge"] * len(dates),
        "source_date": dates,
    },
    {
        "source_name":"FCRebate",
        "source_path":["/app/ecom/FCRebate1"] * 3 + ["/app/ecom/FCRebate2"] * 2,
        "source_date": dates,
    },
]

if __name__ == "__main__":
    for source in souces:
        source_name = source["source_name"]
        source_path = source["source_path"]
        source_date = source["source_date"]
        for i in range(len(source_path)):
            sp = source_path[i]
            sd = source_date[i]
            print "%s_%s_size=`du -sh %s/%s/`" % (source_name, sd, sp, sd)
            print "%s_%s_count=`ls -l %s/%s/*/*/* | grep \"^-\" | wc -l`" % (source_name, sd, sp, sd)
            print "%s_%s_finish=`ls -l %s/%s/* | awk '{print $6 $7}' | sort | tail -1`" % (source_name, sd, sp, sd)
    print "Done"
