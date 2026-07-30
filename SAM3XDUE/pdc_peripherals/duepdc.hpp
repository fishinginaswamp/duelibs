#pragma once
#ifndef PDC_STREAMABLE
#define PDC_STREAMABLE

#include "SAM3XDUE.H"
#include <concepts>


template<typename T>
concept PDC_has_base = requires(T child){
    { child._base } -> std::convertible_to<volatile uint32_t*>;
    { child._isr_offset };
    { child._ier_offset };
    { child._idr_offset };
};

//NOTE: the offset is in words (4 bytes)
//you will need to take the datasheet offset (bytes) and divide by 4
template<typename T>
concept PDC_tx_compatible = PDC_has_base<T> && requires(T child) {
    { child._endtx_mask };
    { child._txbufe_mask };
};

template<typename T>
concept PDC_rx_compatible = PDC_has_base<T> && requires(T child) {
    { child._endrx_mask };
    { child._rxbufe_mask };
};

template <typename T>
concept PDC_compatible = PDC_tx_compatible<T> && PDC_rx_compatible<T>;

template <typename Derived>
class PDC_tx_streamable {
private:
    void (*tx_callback)() = nullptr;

public:
    bool queue_write(const void* src, uint32_t cnt) requires PDC_tx_compatible<Derived> {
        Derived* child = static_cast<Derived*>(this);
        volatile uint32_t* base = child->_base;

        if (*(base + 0x43) == 0) { 
            *(base + 0x42) = reinterpret_cast<uint32_t>(src);
            *(base + 0x43) = cnt;
            enable_tx_interrupt();
            return true;
        }
        else if(*(base + 0x47) == 0){
            *(base + 0x46) = reinterpret_cast<uint32_t>(src);
            *(base + 0x47) = cnt;
            enable_tx_interrupt();
            return true;
        }
        return false;
    }

    void set_tx_callback(void (*callback)()) {
        tx_callback = callback;
    }

    void handle() requires PDC_tx_compatible<Derived> {
        Derived* child = static_cast<Derived*>(this);
        uint32_t sr = *(child->_base + child->_isr_offset); 

        if (sr & child->_endtx_mask) { 
            // Accessed directly from this parent class
            if (tx_callback) (*tx_callback)();
        }
        if (sr & child->_txbufe_mask) { 
            child->disable_tx_interrupt(); 
        }
    }

    void enable_tx_interrupt() requires PDC_tx_compatible<Derived> {
        Derived* child = static_cast<Derived*>(this);
        *(child->_base + child->_ier_offset) = (child->_endtx_mask);
    }
    void disable_tx_interrupt() requires PDC_tx_compatible<Derived> {
        Derived* child = static_cast<Derived*>(this);
        *(child->_base + child->_idr_offset) = (child->_endtx_mask);
    }
};

template <typename Derived>
class PDC_rx_streamable {
        void (*rx_callback)() = nullptr;

public:
    bool queue_read(void* volatile dst, uint32_t cnt) requires PDC_rx_compatible<Derived> {
        Derived* child = static_cast<Derived*>(this);
        volatile uint32_t* base = child->_base;

        if (*(base + 0x41) == 0) {
            *(base + 0x40) = reinterpret_cast<uint32_t>(dst);
            *(base + 0x41) = cnt;
            enable_rx_interrupt();
            return true;
        }
        else if (*(base + 0x45) == 0) { 
            *(base + 0x44) = reinterpret_cast<uint32_t>(dst);
            *(base + 0x45) = cnt;
            enable_rx_interrupt();
            return true;
        }
        return false;
    }

    void set_rx_callback(void (*callback)()) {
        rx_callback = callback;
    }

    void handle() requires PDC_rx_compatible<Derived> {
        Derived* child = static_cast<Derived*>(this);
        uint32_t sr = *(child->_base + child->_isr_offset); 

        if (sr & child->_endrx_mask) { 
            // Accessed directly from this parent class
            if (rx_callback) (*rx_callback)();	
        }
        if (sr & child->_rxbufe_mask) { 
            child->disable_rx_interrupt();
        }
    }

    void enable_rx_interrupt() requires PDC_rx_compatible<Derived> {
        Derived* child = static_cast<Derived*>(this);
        *(child->_base + child->_ier_offset) = (child->_endrx_mask);
    }
    void disable_rx_interrupt() requires PDC_rx_compatible<Derived> {
        Derived* child = static_cast<Derived*>(this);
        *(child->_base + child->_idr_offset) = (child->_endrx_mask);
    }
};

template<typename Derived>
class PDC_streamable : public PDC_rx_streamable<Derived>, public PDC_tx_streamable<Derived>
{
public:
    void handle() requires PDC_compatible<Derived> {
        PDC_tx_streamable<Derived>::handle();
        PDC_rx_streamable<Derived>::handle();
    }
};

#endif