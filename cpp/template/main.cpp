#include "temp.h"

int main () {
    BaseSharder<A> shardera;
    shardera.init();
    shardera.run();

    BaseSharder<B> sharderb;
    sharderb.init();
    sharderb.run();

    MySharder sharderc;
    sharderc.init();
    sharderc.run();

    return 0;
}
