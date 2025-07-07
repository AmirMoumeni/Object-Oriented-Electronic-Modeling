#include <systemc.h>
#include <array>
#include <stack>

using data_packet = std::array<sc_lv<8>, 9>;

class payload_if : virtual public sc_interface {
public:
    virtual void send(const data_packet& pkt) = 0;
    virtual void receive(data_packet& pkt) = 0;
};

class packet_stream : public sc_channel, public payload_if {
private:
    std::stack<data_packet> pkt_buf;
    sc_event evt_ready;

public:
    packet_stream(sc_module_name nm) : sc_channel(nm) {}

    void send(const data_packet& pkt) override {
        pkt_buf.push(pkt);
        evt_ready.notify(SC_ZERO_TIME);
    }

    void receive(data_packet& pkt) override {
        while (pkt_buf.empty()) {
            wait(evt_ready);
        }
        pkt = pkt_buf.top();
        pkt_buf.pop();
    }
};
