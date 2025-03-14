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
    cout << "00 : " << a->value() <<endl;
    b->setValue(ONE);
    cout << "01 : " << b->value() <<endl;
    c->setValue(ONE);
    cout << "10 : " << c->value() <<endl;
    d->setValue(ZERO);
    cout << "11 : " << d->value() <<endl;
    s1->setValue(ZERO);
    s2->setValue(ZERO);
    cout << "Sel : " << s1->value() << s2->value() <<endl;
    Mux* my_mux = new Mux(*s1, *s2, *a, *b, *c, *d, *out, 0);
    my_mux->evl();

    cout << "output : " << out->value() << endl;

    return 0;
}
