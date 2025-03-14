#include "global.hpp"
#include "mux.hpp"
#include "wire.hpp"

int main() {

    Wire *a = new Wire();
    Wire *b = new Wire();
    Wire *c = new Wire();
    Wire *d = new Wire();
    Wire *s1 = new Wire();
    Wire *s2 = new Wire();
    Wire *out = new Wire();

    a->setValue(ZERO);
    b->setValue(ONE);
    c->setValue(ONE);
    d->setValue(ZERO);
    s1->setValue(ZERO);
    s2->setValue(ONE);

    Mux* my_mux = new Mux(*s1, *s2, *a, *b, *c, *d, *out, 0);
    my_mux->evl();

    cout << "output : " << out->value();

    return 0;
}